Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports Crownwood.Magic.Controls
Imports Crownwood.Magic.Common
Imports Crownwood.Magic.Docking
Imports Crownwood.Magic.Menus
Imports AnimatTools.Framework

Namespace DataObjects.Physical

    Public MustInherit Class RigidBody
        Inherits Physical.BodyPart

#Region " Attributes "

        Protected m_JointToParent As DataObjects.Physical.Joint
        Protected m_aryChildBodies As New Collections.SortedRigidBodies(Me)

        Protected m_bFreeze As Boolean
        Protected m_bVisible As Boolean = True
        Protected m_bContactSensor As Boolean
        Protected m_snDensity As ScaledNumber
        Protected m_strTexture As String = ""
        Protected m_imgTexture As Bitmap
        Protected m_imgAlphaTexture As Bitmap

        Protected m_snXCenterOfMass As ScaledNumber
        Protected m_snYCenterOfMass As ScaledNumber
        Protected m_snZCenterOfMass As ScaledNumber

        Protected m_fltCd As Single = 1
        Protected m_fltCdr As Single = 0.5
        Protected m_fltCa As Single = 1
        Protected m_fltCar As Single = 1

        Protected m_snReceptiveFieldDistance As ScaledNumber
        Protected m_gnReceptiveFieldGain As Gain
        Protected m_gnReceptiveCurrentGain As Gain
        Protected m_aryReceptiveFields As New ArrayList
        Protected m_aryReceptiveFieldPairs As New Collections.AnimatSortedList(Me)
        Protected m_vSelectedReceptiveField As Vec3d    'The vertex of the selected receptive field.

        Protected m_aryOdorSources As New Collections.SortedOdors(Me)

        Protected m_bFoodSource As Boolean = False
        Protected m_snFoodQuantity As ScaledNumber
        Protected m_snFoodReplenishRate As ScaledNumber
        Protected m_snFoodEnergyContent As ScaledNumber
        Protected m_snMaxFoodQuantity As ScaledNumber

        Protected m_bIsRoot As Boolean = False

#End Region

#Region " Properties "

        Public Overridable Property JointToParent() As DataObjects.Physical.Joint
            Get
                Return m_JointToParent
            End Get
            Set(ByVal Value As DataObjects.Physical.Joint)
                m_JointToParent = Value
            End Set
        End Property

        Public Overridable ReadOnly Property ChildBodies() As Collections.SortedRigidBodies
            Get
                Return m_aryChildBodies
            End Get
        End Property

        Public Overrides ReadOnly Property BodyPartType() As String
            Get
                Return "RigidBody"
            End Get
        End Property

        Public Overridable Property Freeze() As Boolean
            Get
                Return m_bFreeze
            End Get
            Set(ByVal Value As Boolean)
                If m_JointToParent Is Nothing Then
                    m_bFreeze = Value
                Else
                    Throw New System.Exception("You can only freeze the root body of a structure.")
                End If
            End Set
        End Property

        Public Overridable Property Visible() As Boolean
            Get
                Return m_bVisible
            End Get
            Set(ByVal Value As Boolean)
                m_bVisible = Value
            End Set
        End Property

        Public Overridable Property ContactSensor() As Boolean
            Get
                Return m_bContactSensor
            End Get
            Set(ByVal Value As Boolean)
                m_bContactSensor = Value
            End Set
        End Property

        Public Overridable Property XCenterOfMass() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snXCenterOfMass
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Not Value Is Nothing Then
                    m_snXCenterOfMass.SetFromValue(Value.ActualValue, Util.Environment.DistanceUnits)
                End If
            End Set
        End Property

        Public Overridable Property YCenterOfMass() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snYCenterOfMass
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Not Value Is Nothing Then
                    m_snYCenterOfMass.SetFromValue(Value.ActualValue, Util.Environment.DistanceUnits)
                End If
            End Set
        End Property

        Public Overridable Property ZCenterOfMass() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snZCenterOfMass
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Not Value Is Nothing Then
                    m_snZCenterOfMass.SetFromValue(Value.ActualValue, Util.Environment.DistanceUnits)
                End If
            End Set
        End Property

        Public Overridable Property Density() As ScaledNumber
            Get
                Return m_snDensity
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue <= 0 Then
                    Throw New System.Exception("The density can not be less than or equal to zero.")
                End If

                m_snDensity.CopyData(Value)
            End Set
        End Property

        Public Overridable Property Cd() As Single
            Get
                Return m_fltCd
            End Get
            Set(ByVal Value As Single)
                If Value < 0 Then
                    Throw New System.Exception("The translational drag coefficient (Cd) can not be less than to zero.")
                End If

                m_fltCd = Value
            End Set
        End Property

        Public Overridable Property Cdr() As Single
            Get
                Return m_fltCdr
            End Get
            Set(ByVal Value As Single)
                If Value < 0 Then
                    Throw New System.Exception("The angular drag coefficient (Cdr) can not be less than to zero.")
                End If

                m_fltCdr = Value
            End Set
        End Property

        Public Overridable Property Ca() As Single
            Get
                Return m_fltCa
            End Get
            Set(ByVal Value As Single)
                If Value < 0 Then
                    Throw New System.Exception("The translational added mass coefficient (Ca) can not be less than to zero.")
                End If

                m_fltCa = Value
            End Set
        End Property

        Public Overridable Property Car() As Single
            Get
                Return m_fltCar
            End Get
            Set(ByVal Value As Single)
                If Value < 0 Then
                    Throw New System.Exception("The angular added mass coefficient (Car) can not be less than to zero.")
                End If

                m_fltCar = Value
            End Set
        End Property

        Public Overridable Property ReceptiveFieldDistance() As ScaledNumber
            Get
                Return m_snReceptiveFieldDistance
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue <= 0 Then
                    Throw New System.Exception("The receptive field distance can not be less than or equal to zero.")
                End If

                m_snReceptiveFieldDistance.CopyData(Value)
            End Set
        End Property

        Public Overridable Property ReceptiveFieldGain() As Gain
            Get
                Return m_gnReceptiveFieldGain
            End Get
            Set(ByVal Value As Gain)
                If Not Value Is Nothing Then
                    m_gnReceptiveFieldGain = Value
                Else
                    Throw New System.Exception("You can not set the receptive field gain to null.")
                End If
            End Set
        End Property

        Public Overridable Property ReceptiveCurrentGain() As Gain
            Get
                Return m_gnReceptiveCurrentGain
            End Get
            Set(ByVal Value As Gain)
                If Not Value Is Nothing Then
                    m_gnReceptiveCurrentGain = Value
                Else
                    Throw New System.Exception("You can not set the receptive current gain to null.")
                End If
            End Set
        End Property

        Public Overridable ReadOnly Property ReceptiveFields() As ArrayList
            Get
                Return m_aryReceptiveFields
            End Get
        End Property

        Public Overridable ReadOnly Property ReceptiveFieldPairs() As Collections.AnimatSortedList
            Get
                Return m_aryReceptiveFieldPairs
            End Get
        End Property

        Public Overridable Property SelectedReceptiveField() As Vec3d
            Get
                Return m_vSelectedReceptiveField
            End Get
            Set(ByVal Value As Vec3d)
                m_vSelectedReceptiveField = Value
            End Set
        End Property

        Public Overridable ReadOnly Property CanBeRootBody() As Boolean
            Get
                Return True
            End Get
        End Property

        Public Overridable ReadOnly Property UsesAJoint() As Boolean
            Get
                Return True
            End Get
        End Property

        Public Overridable ReadOnly Property AllowCollisions() As Boolean
            Get
                Return True
            End Get
        End Property

        Public Overridable Property IsRoot() As Boolean
            Get
                Return m_bIsRoot
            End Get
            Set(ByVal Value As Boolean)
                m_bIsRoot = Value
            End Set
        End Property

        Public Overridable Property Texture() As String
            Get
                Return m_strTexture
            End Get
            Set(ByVal Value As String)
                If Not Value Is Nothing Then
                    Dim strPath As String, strFile As String
                    If Util.DetermineFilePath(Value, strPath, strFile) Then
                        Value = strFile
                    End If
                End If

                LoadTextureImage(Value)

                m_strTexture = Value

            End Set
        End Property

        Public Overrides Property PartPositionState() As PartPositionState
            Get
                Dim psState As New PartPositionState

                psState.m_strPartID = m_strID
                psState.m_infoPart = Me.PartPositionInfo

                If Not m_JointToParent Is Nothing Then
                    psState.m_infoJoint = m_JointToParent.PartPositionInfo
                End If

                Dim dbChild As AnimatTools.DataObjects.Physical.RigidBody
                For Each deEntry As DictionaryEntry In m_aryChildBodies
                    dbChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
                    psState.m_aryChildInfos.Add(dbChild.PartPositionInfo)
                Next

                Return psState
            End Get
            Set(ByVal Value As PartPositionState)
                If Not Value Is Nothing AndAlso Value.m_strPartID = m_strID Then
                    Me.PartPositionInfo = Value.m_infoPart

                    If Not m_JointToParent Is Nothing Then
                        m_JointToParent.PartPositionInfo = Value.m_infoJoint
                    End If

                    Dim dbChild As AnimatTools.DataObjects.Physical.RigidBody
                    For Each psChildInfo As PartPositionInfo In Value.m_aryChildInfos
                        dbChild = DirectCast(Me.FindBodyPart(psChildInfo.m_strID), AnimatTools.DataObjects.Physical.RigidBody)

                        If Not dbChild Is Nothing Then
                            dbChild.PartPositionInfo = psChildInfo
                        End If
                    Next
                End If
            End Set
        End Property

        Public Overridable Property OdorSources() As Collections.SortedOdors
            Get
                Return m_aryOdorSources
            End Get
            Set(ByVal Value As Collections.SortedOdors)
                If Not Value Is Nothing Then
                    m_aryOdorSources = Value
                End If
            End Set
        End Property

        Public Overridable Property FoodSource() As Boolean
            Get
                Return m_bFoodSource
            End Get
            Set(ByVal Value As Boolean)
                m_bFoodSource = Value

                'If we change this property it could change the readonly settings of other properties so lets refresh the property grid.
                If Not Me.ParentStructure Is Nothing AndAlso Not Me.ParentStructure.BodyEditor Is Nothing _
                   AndAlso Not Me.ParentStructure.BodyEditor.HierarchyBar Is Nothing AndAlso _
                   TypeOf Me.ParentStructure.BodyEditor.HierarchyBar.TreeView.SelectedNode.Tag Is AnimatTools.Framework.DataObject Then

                    Dim doNode As AnimatTools.Framework.DataObject = DirectCast(Me.ParentStructure.BodyEditor.HierarchyBar.TreeView.SelectedNode.Tag, AnimatTools.Framework.DataObject)
                    Me.ParentStructure.BodyEditor.PropertiesBar.PropertyData = doNode.Properties
                End If

            End Set
        End Property

        Public Overridable Property FoodQuantity() As ScaledNumber
            Get
                Return m_snFoodQuantity
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("The food quantity can not be less than zero.")
                End If

                m_snFoodQuantity.CopyData(Value)
            End Set
        End Property

        Public Overridable Property MaxFoodQuantity() As ScaledNumber
            Get
                Return m_snMaxFoodQuantity
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("The max food quantity can not be less than zero.")
                End If

                m_snMaxFoodQuantity.CopyData(Value)
            End Set
        End Property

        Public Overridable Property FoodReplenishRate() As ScaledNumber
            Get
                Return m_snFoodReplenishRate
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("The food replenish rate can not be less than zero.")
                End If

                m_snFoodReplenishRate.CopyData(Value)
            End Set
        End Property

        Public Overridable Property FoodEnergyContent() As ScaledNumber
            Get
                Return m_snFoodEnergyContent
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("The food Eenergy content can not be less than zero.")
                End If

                m_snFoodEnergyContent.CopyData(Value)
            End Set
        End Property

        Public MustOverride Property Color() As System.Drawing.Color
        Public MustOverride Property Alpha() As Integer
        Public MustOverride Property Transparency() As Single

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            m_thDataTypes.DataTypes.Clear()

            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyTorqueX", "Torque X Axis", "Newton-Meters", "Nm", -1000, 1000))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyTorqueY", "Torque Y Axis", "Newton-Meters", "Nm", -1000, 1000))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyTorqueZ", "Torque Z Axis", "Newton-Meters", "Nm", -1000, 1000))

            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyForceX", "Force X Axis", "Newtons", "N", -1000, 1000))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyForceY", "Force Y Axis", "Newtons", "N", -1000, 1000))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyForceZ", "Force Z Axis", "Newtons", "N", -1000, 1000))

            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyPositionX", "Position X Axis", "Meters", "m", -10, 10))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyPositionY", "Position Y Axis", "Meters", "m", -10, 10))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyPositionZ", "Position Z Axis", "Meters", "m", -10, 10))

            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyRotationX", "Rotation X Axis", "Radians", "rad", -4, 4))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyRotationY", "Rotation Y Axis", "Radians", "rad", -4, 4))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyRotationZ", "Rotation Z Axis", "Radians", "rad", -4, 4))

            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyLinearVelocityX", "Linear Velocity X Axis", "m/s", "m/s", -5, 5))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyLinearVelocityY", "Linear Velocity Y Axis", "m/s", "m/s", -5, 5))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyLinearVelocityZ", "Linear Velocity Z Axis", "m/s", "m/s", -5, 5))

            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyAngularVelocityX", "Angular Velocity X Axis", "rad/s", "rad/s", -5, 5))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyAngularVelocityY", "Angular Velocity Y Axis", "rad/s", "rad/s", -5, 5))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyAngularVelocityZ", "Angular Velocity Z Axis", "rad/s", "rad/s", -5, 5))

            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyLinearAccelerationX", "Linear Acceleration X Axis", "m/s^2", "m/s^2", -5, 5))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyLinearAccelerationY", "Linear Acceleration Y Axis", "m/s^2", "m/s^2", -5, 5))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyLinearAccelerationZ", "Linear Acceleration Z Axis", "m/s^2", "m/s^2", -5, 5))

            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyAngularAccelerationX", "Angular Acceleration X Axis", "rad/s^2", "rad/s^2", -5, 5))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyAngularAccelerationY", "Angular Acceleration Y Axis", "rad/s^2", "rad/s^2", -5, 5))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyAngularAccelerationZ", "Angular Acceleration Z Axis", "rad/s^2", "rad/s^2", -5, 5))

            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyDragForceX", "Drag Force X Axis", "Newtons", "N", -100, 100))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyDragForceY", "Drag Force Y Axis", "Newtons", "N", -100, 100))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyDragForceZ", "Drag Force Z Axis", "Newtons", "N", -100, 100))

            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyDragTorqueX", "Drag Torque X Axis", "Newton-Meters", "Nm", -100, 100))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyDragTorqueY", "Drag Torque Y Axis", "Newton-Meters", "Nm", -100, 100))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyDragTorqueZ", "Drag Torque Z Axis", "Newton-Meters", "Nm", -100, 100))

            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyAddedMassForceX", "Added Mass Force X Axis", "Newtons", "N", -100, 100))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyAddedMassForceY", "Added Mass Force Y Axis", "Newtons", "N", -100, 100))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyAddedMassForceZ", "Added Mass Force Z Axis", "Newtons", "N", -100, 100))

            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyAddedMassTorqueX", "Added Mass Torque X Axis", "Newton-Meters", "Nm", -100, 100))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyAddedMassTorqueY", "Added Mass Torque Y Axis", "Newton-Meters", "Nm", -100, 100))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyAddedMassTorqueZ", "Added Mass Torque Z Axis", "Newton-Meters", "Nm", -100, 100))

            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("BodyBuoyancy", "Buoyancy", "Newtons", "N", 0, 100))

            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("FoodQuantity", "Food Quantity", "", "", 0, 1000))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("FoodEaten", "Food Eaten", "", "", 0, 1000))

            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("Mass", "Mass", "Kilograms", "Kg", -5000, 5000))
            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("Volume", "Volume", "Cubic Meters", "m^3", -100, 100))

            m_thDataTypes.ID = "BodyForceX"

            AddCompatibleStimulus(New AnimatTools.DataObjects.ExternalStimuli.Force(Nothing))

            m_thIncomingDataType = New AnimatTools.DataObjects.DataType("BodyForceX", "Body Force X", "Newtons", "N", -100, 100, ScaledNumber.enumNumericScale.None, ScaledNumber.enumNumericScale.None)

            m_snReceptiveFieldDistance = New ScaledNumber(Me, "RecptiveFieldDistance", 25, ScaledNumber.enumNumericScale.centi, "Meters", "m")

            m_snXCenterOfMass = New AnimatTools.Framework.ScaledNumber(Me, "XCenterOfMass", "meters", "m")
            m_snYCenterOfMass = New AnimatTools.Framework.ScaledNumber(Me, "YCenterOfMass", "meters", "m")
            m_snZCenterOfMass = New AnimatTools.Framework.ScaledNumber(Me, "ZCenterOfMass", "meters", "m")

            If Not Util.Environment Is Nothing Then
                m_snDensity = Util.Environment.DefaultDensity
                m_snXCenterOfMass.SetFromValue(0, Util.Environment.DistanceUnits)
                m_snYCenterOfMass.SetFromValue(0, Util.Environment.DistanceUnits)
                m_snZCenterOfMass.SetFromValue(0, Util.Environment.DistanceUnits)
            Else
                m_snDensity = New ScaledNumber(Me, "Density", 1000, ScaledNumber.enumNumericScale.Kilo, "g/m^2", "g/m^2")
            End If

            m_snFoodQuantity = New ScaledNumber(Me, "FoodQuantity", 100, ScaledNumber.enumNumericScale.None, "", "")
            m_snMaxFoodQuantity = New ScaledNumber(Me, "MaxFoodQuantity", 10, ScaledNumber.enumNumericScale.Kilo, "", "")
            m_snFoodReplenishRate = New ScaledNumber(Me, "FoodReplenishRate", 1, ScaledNumber.enumNumericScale.None, "Quantity/s", "Q/s")
            m_snFoodEnergyContent = New ScaledNumber(Me, "FoodEnergyContent", 1, ScaledNumber.enumNumericScale.Kilo, "Calories/Quantity", "C/Q")

            Dim gnRFGain As New Gains.Bell(Me, "ReceptiveFieldGain", "Meters", "Gain")
            gnRFGain.XOffset.ActualValue = 0
            gnRFGain.Amplitude.ActualValue = 1
            gnRFGain.Width.ActualValue = 10
            gnRFGain.YOffset.ActualValue = 0
            gnRFGain.LowerLimit.ActualValue = -1
            gnRFGain.UpperLimit.ActualValue = 1
            m_gnReceptiveFieldGain = gnRFGain

            Dim gnRCGain As New Gains.Polynomial(Me, "ReceptiveCurrnetGain", "Force", "Current")
            gnRCGain.A.ActualValue = 0
            gnRCGain.B.ActualValue = 0
            gnRCGain.C.ActualValue = 0.000000001
            gnRCGain.D.ActualValue = 0
            gnRCGain.UseLimits = True
            gnRCGain.LowerLimit.ActualValue = 0
            gnRCGain.UpperLimit.ActualValue = 10
            gnRCGain.LowerOutput.ActualValue = 0
            gnRCGain.UpperOutput.SetFromValue(0.00000007, ScaledNumber.enumNumericScale.nano)
            m_gnReceptiveCurrentGain = gnRCGain

            m_vSelectedReceptiveField = New AnimatTools.Framework.Vec3d(Nothing, 0, 0, 0)

        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()
            If Not m_JointToParent Is Nothing Then m_JointToParent.ClearIsDirty()
            m_aryChildBodies.ClearIsDirty()
            m_aryOdorSources.ClearIsDirty()
        End Sub

        Public Overrides Function FindBodyPart(ByVal strID As String) As BodyPart

            Dim bpPart As BodyPart
            If Not m_JointToParent Is Nothing Then bpPart = m_JointToParent.FindBodyPart(strID)
            If Not bpPart Is Nothing Then Return bpPart

            If Me.ID = strID Then Return Me

            Dim dbChild As AnimatTools.DataObjects.Physical.RigidBody
            For Each deEntry As DictionaryEntry In m_aryChildBodies
                dbChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
                bpPart = dbChild.FindBodyPart(strID)
                If Not bpPart Is Nothing Then Return bpPart
            Next

            Return Nothing
        End Function

        Public Overrides Function FindBodyPartByName(ByVal strName As String) As BodyPart

            Dim bpPart As BodyPart
            If Not m_JointToParent Is Nothing Then bpPart = m_JointToParent.FindBodyPartByName(strName)
            If Not bpPart Is Nothing Then Return bpPart

            If Me.Name = strName Then Return Me

            Dim dbChild As AnimatTools.DataObjects.Physical.RigidBody
            For Each deEntry As DictionaryEntry In m_aryChildBodies
                dbChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
                bpPart = dbChild.FindBodyPartByName(strName)
                If Not bpPart Is Nothing Then Return bpPart
            Next

            Return Nothing
        End Function

        Public Overrides Function FindBodyPartByCloneID(ByVal strId As String) As BodyPart

            Dim bpPart As BodyPart
            If Not m_JointToParent Is Nothing Then bpPart = m_JointToParent.FindBodyPartByCloneID(strId)
            If Not bpPart Is Nothing Then Return bpPart

            If Me.CloneID = strId Then Return Me

            Dim dbChild As AnimatTools.DataObjects.Physical.RigidBody
            For Each deEntry As DictionaryEntry In m_aryChildBodies
                dbChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
                bpPart = dbChild.FindBodyPartByCloneID(strId)
                If Not bpPart Is Nothing Then Return bpPart
            Next

            Return Nothing
        End Function

        Public Overloads Overrides Sub FindChildrenOfType(ByVal tpTemplate As System.Type, ByRef colDataObjects As Collections.DataObjects)
            MyBase.FindChildrenOfType(tpTemplate, colDataObjects)

            If Not m_JointToParent Is Nothing Then
                m_JointToParent.FindChildrenOfType(tpTemplate, colDataObjects)
            End If

            Dim dbChild As AnimatTools.DataObjects.Physical.RigidBody
            For Each deEntry As DictionaryEntry In m_aryChildBodies
                dbChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
                dbChild.FindChildrenOfType(tpTemplate, colDataObjects)
            Next

        End Sub

        Public Overrides Function CreateJointTreeView(ByRef tvTree As TreeView, ByVal tnParent As TreeNode, _
                                                      ByVal thSelectedPart As TypeHelpers.LinkedBodyPart) As TreeNode

            Dim tnJoint As TreeNode
            If Not m_JointToParent Is Nothing Then
                tnParent = m_JointToParent.CreateJointTreeView(tvTree, tnParent, thSelectedPart)
            End If

            Dim dbChild As AnimatTools.DataObjects.Physical.RigidBody
            For Each deEntry As DictionaryEntry In m_aryChildBodies
                dbChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
                dbChild.CreateJointTreeView(tvTree, tnParent, thSelectedPart)
            Next

        End Function

        Public Overrides Function CreateRigidBodyTreeView(ByRef tvTree As TreeView, ByVal tnParent As TreeNode, _
                                                          ByVal thSelectedPart As TypeHelpers.LinkedBodyPart) As TreeNode

            Dim tnBody As TreeNode
            tnBody = tnParent.Nodes.Add(Me.Name)
            tnBody.ForeColor = Color.Red
            Dim thPart As TypeHelpers.LinkedBodyPart = DirectCast(thSelectedPart.Clone(thSelectedPart.Parent, False, Nothing), TypeHelpers.LinkedBodyPart)
            thPart.BodyPart = Me
            tnBody.Tag = thPart

            Dim dbChild As AnimatTools.DataObjects.Physical.RigidBody
            For Each deEntry As DictionaryEntry In m_aryChildBodies
                dbChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
                dbChild.CreateRigidBodyTreeView(tvTree, tnBody, thSelectedPart)
            Next

        End Function

        Public Overrides Sub CreateBodyPlanTreeView(ByVal dsSim As AnimatTools.DataObjects.Simulation, _
                                                      ByVal doStructure As DataObjects.Physical.PhysicalStructure, _
                                                      ByVal doParentNode As DataObjects.Physical.BodyPart, _
                                                      ByVal frmHierarchy As Forms.BodyPlan.Hierarchy)

            If Not m_JointToParent Is Nothing Then
                m_JointToParent.CreateBodyPlanTreeView(dsSim, doStructure, doParentNode, frmHierarchy)
            End If

            If m_JointToParent Is Nothing Then
                If doStructure.RootBody Is Me Then
                    MyBase.CreateBodyPlanTreeView(dsSim, doStructure, Nothing, frmHierarchy)
                Else
                    MyBase.CreateBodyPlanTreeView(dsSim, doStructure, doParentNode, frmHierarchy)
                End If
            Else
                MyBase.CreateBodyPlanTreeView(dsSim, doStructure, m_JointToParent, frmHierarchy)
            End If

            Dim dbChild As AnimatTools.DataObjects.Physical.RigidBody
            For Each deEntry As DictionaryEntry In m_aryChildBodies
                dbChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
                dbChild.CreateBodyPlanTreeView(dsSim, doStructure, Me, frmHierarchy)
            Next

        End Sub

        Public Overrides Sub RemoveBodyPlanTreeView()

            If Not m_JointToParent Is Nothing Then
                If Not m_JointToParent.BodyPlanTreeNode Is Nothing Then
                    m_JointToParent.RemoveBodyPlanTreeView()
                End If
            Else
                If Not m_bpBodyNode Is Nothing Then
                    m_bpBodyNode.Remove()
                End If
            End If

            m_bpBodyNode = Nothing
        End Sub

        Public Overrides Function BodyPlanTreeviewPopupMenu(ByRef tnSelectedNode As TreeNode, ByVal ptPoint As Point) As Boolean

            If tnSelectedNode Is m_bpBodyNode Then
                Return MyBase.BodyPlanTreeviewPopupMenu(tnSelectedNode, ptPoint)
            Else
                If Not m_JointToParent Is Nothing Then
                    If m_JointToParent.BodyPlanTreeviewPopupMenu(tnSelectedNode, ptPoint) Then
                        Return True
                    End If
                End If

                Dim doChild As AnimatTools.DataObjects.Physical.RigidBody
                For Each deEntry As DictionaryEntry In m_aryChildBodies
                    doChild = DirectCast(deEntry.Value, DataObjects.Physical.RigidBody)
                    If doChild.BodyPlanTreeviewPopupMenu(tnSelectedNode, ptPoint) Then
                        Return True
                    End If
                Next
            End If

            Return False
        End Function

        Public Overrides Function CreateDataItemTreeView(ByVal frmDataItem As Forms.Tools.SelectDataItem, ByVal tnParent As TreeNode, ByVal tpTemplatePartType As Type) As TreeNode

            If tpTemplatePartType Is Nothing OrElse (Not tpTemplatePartType Is Nothing AndAlso Util.IsTypeOf(Me.GetType(), tpTemplatePartType, False)) Then
                Dim tnJointToParent As TreeNode
                If Not m_JointToParent Is Nothing Then
                    tnJointToParent = m_JointToParent.CreateDataItemTreeView(frmDataItem, tnParent, tpTemplatePartType)
                End If

                Dim tnNewParent As TreeNode
                If m_JointToParent Is Nothing Then
                    tnNewParent = MyBase.CreateDataItemTreeView(frmDataItem, tnParent, tpTemplatePartType)
                Else
                    tnNewParent = MyBase.CreateDataItemTreeView(frmDataItem, tnJointToParent, tpTemplatePartType)
                End If

                Dim dbChild As AnimatTools.DataObjects.Physical.RigidBody
                For Each deEntry As DictionaryEntry In m_aryChildBodies
                    dbChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
                    dbChild.CreateDataItemTreeView(frmDataItem, tnNewParent, tpTemplatePartType)
                Next
            End If

        End Function

        Protected Overridable Sub LoadTextureImage(ByVal strTexture As String)

            If strTexture.Trim.Length > 0 Then
                Dim strExt As String = Util.GetFileExtension(strTexture)

                If strExt.Trim.ToUpper() <> "BMP" Then
                    Throw New System.Exception("Texture files must be .bmp type files.")
                End If

                If Util.IsFullPath(strTexture) Then
                    m_imgTexture = DirectCast(Image.FromFile(strTexture), Bitmap)
                Else
                    m_imgTexture = DirectCast(Image.FromFile(Util.Application.ProjectPath + strTexture), Bitmap)
                End If

                If Not ((m_imgTexture.Size.Width = 128 OrElse m_imgTexture.Size.Width = 256 OrElse m_imgTexture.Size.Width = 512) _
                   AndAlso (m_imgTexture.Size.Height = 128 OrElse m_imgTexture.Size.Height = 256 OrElse m_imgTexture.Size.Height = 512)) Then
                    Throw New System.Exception("Texture files width and height must be multiples of 2^n (128, 256, 512).")
                End If

                Dim bmpManipulator As New Crownwood.Magic.Controls.BitmapManipulator
                Dim imgDest As Bitmap

                imgDest = New Bitmap(m_imgTexture.Width, m_imgTexture.Height, Imaging.PixelFormat.Format32bppArgb)
                m_imgAlphaTexture = bmpManipulator.OverlayBitmap(imgDest, m_imgTexture, 64, New Point(0, 0))

                If Me.Alpha < 255 Then
                    imgDest = New Bitmap(m_imgTexture.Width, m_imgTexture.Height, Imaging.PixelFormat.Format32bppArgb)
                    m_imgTexture = bmpManipulator.OverlayBitmap(imgDest, m_imgTexture, Me.Alpha, New Point(0, 0))
                End If
            Else
                m_imgTexture = Nothing
            End If

        End Sub

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()

            If Util.Simulation.Environment.SimulateHydrodynamics Then
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Cd", m_fltCd.GetType(), "Cd", _
                                            "Hydrodynamics", "The translational drag coefficient of this body.", m_fltCd))

                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Cdr", m_fltCdr.GetType(), "Cdr", _
                                 "Hydrodynamics", "The angular drag coefficient of this body.", m_fltCdr))

                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Ca", m_fltCa.GetType(), "Ca", _
                                 "Hydrodynamics", "The translational added mass coefficient of this body.", m_fltCa))

                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Car", m_fltCar.GetType(), "Car", _
                                 "Hydrodynamics", "The angular added mass coefficient of this body.", m_fltCar))
            End If

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Visible", m_bVisible.GetType(), "Visible", _
                                        "Part Properties", "Sets whether or not this part is visible in the simulation.", m_bVisible))

            'm_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Contact Sensor", m_bContactSensor.GetType(), "ContactSensor", _
            '                            "Part Properties", "Sets whether or not this part can detect contacts.", m_bContactSensor))

            ' m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Alpha", GetType(Integer), "Alpha", _
            '                            "Part Properties", "Determines the transparency for this object. Valid values are between 0 and 255.", Me.Alpha))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Transparency", GetType(Single), "Transparency", _
                                       "Part Properties", "Determines the transparency for this object. Valid values are between 0 (fully transparent) and 1 (fully opaque).", Me.Alpha))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Texture", GetType(String), "Texture", _
                                        "Part Properties", "Sets the bmp texture file to wrap onto this body part.", Me.Texture, GetType(System.Windows.Forms.Design.FileNameEditor)))

            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snDensity.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Density", pbNumberBag.GetType(), "Density", _
                                        "Part Properties", "Sets the density of this body part.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            'Center Of Mass
            pbNumberBag = Me.XLocalLocationScaled.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("X   ", pbNumberBag.GetType(), "XCenterOfMass", _
                                        "Center Of Mass", "Sets the x offset from the center of this object to the new center of mass for dynamics calculations.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = Me.YLocalLocationScaled.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Y   ", pbNumberBag.GetType(), "YCenterOfMass", _
                                        "Center Of Mass", "Sets the y offset from the center of this object to the new center of mass for dynamics calculations.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = Me.ZLocalLocationScaled.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Z   ", pbNumberBag.GetType(), "ZCenterOfMass", _
                                        "Center Of Mass", "Sets the z offset from the center of this object to the new center of mass for dynamics calculations.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            'm_JointToParent Is Nothing
            If m_bIsRoot Then
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Freeze", m_bFreeze.GetType(), "Freeze", _
                                            "Part Properties", "If the root body is frozen then it is locked in place in the environment.", m_bFreeze))
            Else
                'Only show a location if it is not the root object.

                pbNumberBag = Me.XLocalLocationScaled.Properties
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("X  ", pbNumberBag.GetType(), "XLocalLocationScaled", _
                                            "Location Local", "Sets the x location of this body part.", pbNumberBag, _
                                            "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

                pbNumberBag = Me.YLocalLocationScaled.Properties
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Y  ", pbNumberBag.GetType(), "YLocalLocationScaled", _
                                            "Location Local", "Sets the y location of this body part.", pbNumberBag, _
                                            "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

                pbNumberBag = Me.ZLocalLocationScaled.Properties
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Z  ", pbNumberBag.GetType(), "ZLocalLocationScaled", _
                                            "Location Local", "Sets the z location of this body part.", pbNumberBag, _
                                            "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

                pbNumberBag = Me.XWorldLocationScaled.Properties
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("X ", pbNumberBag.GetType(), "XWorldLocationScaled", _
                                            "Location World", "Sets the x location of this body part.", pbNumberBag, _
                                            "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

                pbNumberBag = Me.YWorldLocationScaled.Properties
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Y ", pbNumberBag.GetType(), "YWorldLocationScaled", _
                                            "Location World", "Sets the y location of this body part.", pbNumberBag, _
                                            "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

                pbNumberBag = Me.ZWorldLocationScaled.Properties
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Z ", pbNumberBag.GetType(), "ZWorldLocationScaled", _
                                            "Location World", "Sets the z location of this body part.", pbNumberBag, _
                                            "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))
            End If

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Odor Sources", m_aryOdorSources.GetType(), "OdorSources", _
                                        "Part Properties", "Edit the odor sources that this part can emit.", m_aryOdorSources, _
                                        GetType(TypeHelpers.OdorTypeEditor), GetType(TypeHelpers.OdorTypeConverter)))

            If m_bFoodSource Then
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("FoodSource", m_bFoodSource.GetType(), "FoodSource", _
                                            "Food Properties", "Determines whether this rigid body is a food source."))

                pbNumberBag = Me.m_snFoodQuantity.Properties
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Food Quantity", pbNumberBag.GetType(), "FoodQuantity", _
                                            "Food Properties", "Sets the initial quantity of food available at this food source.", pbNumberBag, _
                                            "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

                pbNumberBag = Me.m_snMaxFoodQuantity.Properties
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Max Food Quantity", pbNumberBag.GetType(), "MaxFoodQuantity", _
                                            "Food Properties", "Sets the maximum quantity of food available at this food source.", pbNumberBag, _
                                            "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

                pbNumberBag = Me.m_snFoodReplenishRate.Properties
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Food Replenish Rate", pbNumberBag.GetType(), "FoodReplenishRate", _
                                            "Food Properties", "Sets the rate at which food is replensihed in this food source.", pbNumberBag, _
                                            "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

                pbNumberBag = Me.m_snFoodEnergyContent.Properties
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Food Energy Content", pbNumberBag.GetType(), "FoodEnergyContent", _
                                            "Food Properties", "Sets the calorie content for each piece of food.", pbNumberBag, _
                                            "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))
            Else
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("FoodSource", m_bFoodSource.GetType(), "FoodSource", _
                                            "Part Properties", "Determines whether this rigid body is a food source."))
            End If

        End Sub

        Public Overrides Sub AfterClone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal doClone As AnimatTools.Framework.DataObject)
            MyBase.AfterClone(doParent, bCutData, doOriginal, doClone)

            If Not m_JointToParent Is Nothing Then
                m_JointToParent.AfterClone(Me, bCutData, doOriginal, doClone)
            End If

            Dim doChild As RigidBody
            For Each deItem As DictionaryEntry In Me.m_aryChildBodies
                doChild = DirectCast(deItem.Value, RigidBody)
                doChild.AfterClone(Me, bCutData, doOriginal, doClone)
            Next

        End Sub

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim doOrigPart As RigidBody = DirectCast(doOriginal, RigidBody)

            If Not doOrigPart.m_JointToParent Is Nothing Then
                m_JointToParent = DirectCast(doOrigPart.m_JointToParent.Clone(doOrigPart.m_JointToParent.Parent, bCutData, doRoot), Joint)
            Else
                m_JointToParent = Nothing
            End If

            m_bFreeze = False   '' only the root object can be frozen.
            m_bVisible = doOrigPart.m_bVisible
            m_bContactSensor = doOrigPart.m_bContactSensor
            m_snDensity = DirectCast(doOrigPart.m_snDensity, ScaledNumber)
            m_snReceptiveFieldDistance = DirectCast(doOrigPart.m_snReceptiveFieldDistance, ScaledNumber)
            m_fltCd = doOrigPart.m_fltCd
            m_fltCdr = doOrigPart.m_fltCdr
            m_fltCa = doOrigPart.m_fltCa
            m_fltCar = doOrigPart.m_fltCar
            m_bIsRoot = doOrigPart.m_bIsRoot
            m_bFoodSource = doOrigPart.m_bFoodSource
            m_snFoodQuantity = DirectCast(doOrigPart.m_snFoodQuantity, ScaledNumber)
            m_snMaxFoodQuantity = DirectCast(doOrigPart.m_snMaxFoodQuantity, ScaledNumber)
            m_snFoodReplenishRate = DirectCast(doOrigPart.m_snFoodReplenishRate, ScaledNumber)
            m_snFoodEnergyContent = DirectCast(doOrigPart.m_snFoodEnergyContent, ScaledNumber)
            m_snXCenterOfMass = DirectCast(doOrigPart.m_snXCenterOfMass.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snYCenterOfMass = DirectCast(doOrigPart.m_snYCenterOfMass.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snZCenterOfMass = DirectCast(doOrigPart.m_snZCenterOfMass.Clone(Me, bCutData, doRoot), ScaledNumber)

            m_strTexture = doOrigPart.m_strTexture

            If Not doOrigPart.m_imgTexture Is Nothing Then
                m_imgTexture = DirectCast(doOrigPart.m_imgTexture.Clone, Bitmap)
            End If

            If Not doOrigPart.m_imgAlphaTexture Is Nothing Then
                m_imgAlphaTexture = DirectCast(doOrigPart.m_imgAlphaTexture.Clone, Bitmap)
            End If

            m_aryChildBodies.Clear()

            Dim doOrigChild As RigidBody
            Dim doNewChild As RigidBody
            For Each deItem As DictionaryEntry In doOrigPart.m_aryChildBodies
                doOrigChild = DirectCast(deItem.Value, RigidBody)
                doNewChild = DirectCast(doOrigChild.Clone(Me, bCutData, doRoot), RigidBody)
                m_aryChildBodies.Add(doNewChild.ID, doNewChild)
            Next

            m_aryOdorSources = DirectCast(doOrigPart.m_aryOdorSources.Clone(Me, bCutData, doRoot), Collections.SortedOdors)

        End Sub

        Public Overrides Sub RenameBodyParts(ByVal doStructure As AnimatTools.DataObjects.Physical.PhysicalStructure)

            Me.m_strID = System.Guid.NewGuid.ToString()

            Try
                doStructure.NewBodyIndex = doStructure.NewBodyIndex + 1
                Me.Name = "Body_" & doStructure.NewBodyIndex
            Catch ex As System.Exception
                Me.Name = "Body_" & System.Guid.NewGuid.ToString()
            End Try

            If Not m_JointToParent Is Nothing Then
                m_JointToParent.RenameBodyParts(doStructure)
            End If

            Dim doChild As AnimatTools.DataObjects.Physical.RigidBody
            For Each deItem As DictionaryEntry In Me.ChildBodies
                doChild = DirectCast(deItem.Value, AnimatTools.DataObjects.Physical.RigidBody)
                doChild.RenameBodyParts(doStructure)
            Next
        End Sub

        Public Overrides Sub ClearSelectedBodyParts()

            m_bSelected = False

            If Not m_JointToParent Is Nothing Then
                m_JointToParent.ClearSelectedBodyParts()
            End If

            Dim doChild As AnimatTools.DataObjects.Physical.RigidBody
            For Each deItem As DictionaryEntry In Me.ChildBodies
                doChild = DirectCast(deItem.Value, AnimatTools.DataObjects.Physical.RigidBody)
                doChild.ClearSelectedBodyParts()
            Next

        End Sub

        Public Overrides Sub InitializeAfterLoad(ByRef dsSim As Simulation, ByRef doStructure As DataObjects.Physical.PhysicalStructure)

            If Not m_JointToParent Is Nothing Then
                m_JointToParent.InitializeAfterLoad(dsSim, doStructure)
            End If

            Dim doChild As AnimatTools.DataObjects.Physical.RigidBody
            For Each deEntry As DictionaryEntry In m_aryChildBodies
                doChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
                doChild.InitializeAfterLoad(dsSim, doStructure)
            Next

            Dim doPair As ReceptiveFieldPair
            For Each deEntry As DictionaryEntry In m_aryReceptiveFieldPairs
                doPair = DirectCast(deEntry.Value, ReceptiveFieldPair)
                doPair.InitializeAfterLoad(dsSim, doStructure)
            Next


        End Sub

        Public Overloads Overrides Sub LoadData(ByRef dsSim As Simulation, ByRef doStructure As DataObjects.Physical.PhysicalStructure, ByRef oXml As Interfaces.StdXml)
            MyBase.LoadData(dsSim, doStructure, oXml)

            m_aryChildBodies.Clear()

            oXml.IntoElem() 'Into RigidBody Element

            m_bVisible = oXml.GetChildBool("IsVisible", m_bVisible)
            m_bContactSensor = oXml.GetChildBool("IsContactSensor", m_bContactSensor)

            If oXml.FindChildElement("DragCoefficient", False) Then
                m_fltCd = oXml.GetChildFloat("DragCoefficient", m_fltCd)
                m_fltCdr = m_fltCd
            Else
                m_fltCd = oXml.GetChildFloat("Cd", m_fltCd)
                m_fltCdr = oXml.GetChildFloat("Cdr", m_fltCdr)
                m_fltCa = oXml.GetChildFloat("Ca", m_fltCa)
                m_fltCar = oXml.GetChildFloat("Car", m_fltCar)
            End If

            m_strTexture = oXml.GetChildString("Texture", m_strTexture)
            m_strTexture = Util.VerifyFilePath(m_strTexture)

            If ScaledNumber.IsValidXml(oXml, "Density") Then
                m_snDensity.LoadData(oXml, "Density")
            Else
                m_snDensity.ActualValue = oXml.GetChildFloat("Density", CSng(m_snDensity.ActualValue))
            End If

            If oXml.FindChildElement("XCenterOfMass", False) Then
                m_snXCenterOfMass.LoadData(oXml, "XCenterOfMass")
                m_snYCenterOfMass.LoadData(oXml, "YCenterOfMass")
                m_snZCenterOfMass.LoadData(oXml, "ZCenterOfMass")
            End If

            If oXml.FindChildElement("ContactSensor", False) Then
                oXml.IntoElem()

                m_snReceptiveFieldDistance.LoadData(oXml, "ReceptiveFieldDistance")
                m_gnReceptiveFieldGain.LoadData(oXml, "FieldGain", "ReceptiveFieldGain")
                m_gnReceptiveCurrentGain.LoadData(oXml, "CurrentGain", "ReceptiveCurrentGain")

                m_aryReceptiveFieldPairs.Clear()

                oXml.IntoChildElement("FieldPairs")
                Dim doPair As ReceptiveFieldPair
                Dim iCount As Integer = oXml.NumberOfChildren() - 1
                For iIndex As Integer = 0 To iCount
                    oXml.FindChildByIndex(iIndex)

                    doPair = New ReceptiveFieldPair(Me)
                    doPair.LoadData(dsSim, doStructure, oXml)
                    m_aryReceptiveFieldPairs.Add(doPair.ID, doPair)
                Next
                oXml.OutOfElem()   'Outof ChildBodies Element

                oXml.OutOfElem()
            End If

            'If this is the root body element then do not attempt to load the joint. Otherwise it must have a joint
            If Not Me.IsRoot Then
                If Me.UsesAJoint Then
                    If oXml.FindChildElement("Joint", False) Then
                        m_JointToParent = DirectCast(dsSim.CreateObject(oXml, "Joint", Me), DataObjects.Physical.Joint)
                        m_JointToParent.LoadData(dsSim, doStructure, oXml)
                    End If
                End If
            Else
                'If it is the root body then see if we are going to freeze it.
                m_bFreeze = oXml.GetChildBool("Freeze", m_bFreeze)
            End If

            If oXml.FindChildElement("ChildBodies", False) Then
                Dim dbBody As DataObjects.Physical.RigidBody

                oXml.IntoElem() 'Into ChildBodies Element
                Dim iCount As Integer = oXml.NumberOfChildren() - 1
                For iIndex As Integer = 0 To iCount
                    oXml.FindChildByIndex(iIndex)

                    dbBody = DirectCast(dsSim.CreateObject(oXml, "RigidBody", Me), DataObjects.Physical.RigidBody)
                    dbBody.LoadData(dsSim, doStructure, oXml)
                    m_aryChildBodies.Add(dbBody.ID, dbBody)
                Next
                oXml.OutOfElem()   'Outof ChildBodies Element
            End If

            If oXml.FindChildElement("OdorSources", False) Then
                Dim doOdor As DataObjects.Physical.Odor

                oXml.IntoElem() 'Into ChildBodies Element
                Dim iCount As Integer = oXml.NumberOfChildren() - 1
                For iIndex As Integer = 0 To iCount
                    oXml.FindChildByIndex(iIndex)

                    doOdor = New Odor(Me)
                    doOdor.LoadData(dsSim, oXml)

                    If Not doOdor.OdorType Is Nothing Then
                        m_aryOdorSources.Add(doOdor.ID, doOdor)
                    End If
                Next
                oXml.OutOfElem()   'Outof ChildBodies Element
            End If

            m_bFoodSource = oXml.GetChildBool("FoodSource", m_bFoodSource)

            If m_bFoodSource Then
                m_snFoodQuantity.LoadData(oXml, "FoodQuantity")
                m_snMaxFoodQuantity.LoadData(oXml, "MaxFoodQuantity")
                m_snFoodReplenishRate.LoadData(oXml, "FoodReplenishRate")
                m_snFoodEnergyContent.LoadData(oXml, "FoodEnergyContent")
            End If

            oXml.OutOfElem() 'Outof RigidBody Element

        End Sub

        Public Overloads Overrides Sub SaveData(ByRef dsSim As Simulation, ByRef doStructure As DataObjects.Physical.PhysicalStructure, ByRef oXml As Interfaces.StdXml)
            MyBase.SaveData(dsSim, doStructure, oXml)

            oXml.IntoElem() 'Into Child Elemement

            oXml.AddChildElement("IsVisible", m_bVisible)
            oXml.AddChildElement("IsContactSensor", m_bContactSensor)

            oXml.AddChildElement("Cd", m_fltCd)
            oXml.AddChildElement("Cdr", m_fltCdr)
            oXml.AddChildElement("Ca", m_fltCa)
            oXml.AddChildElement("Car", m_fltCar)

            oXml.AddChildElement("Texture", m_strTexture)
            m_snDensity.SaveData(oXml, "Density")
            m_snXCenterOfMass.SaveData(oXml, "XCenterOfMass")
            m_snYCenterOfMass.SaveData(oXml, "YCenterOfMass")
            m_snZCenterOfMass.SaveData(oXml, "ZCenterOfMass")

            If m_aryReceptiveFieldPairs.Count > 0 AndAlso TypeOf doStructure Is AnimatTools.DataObjects.Physical.Organism Then
                oXml.AddChildElement("ContactSensor")
                oXml.IntoElem()

                m_snReceptiveFieldDistance.SaveData(oXml, "ReceptiveFieldDistance")
                m_gnReceptiveFieldGain.SaveData(oXml, "FieldGain")
                m_gnReceptiveCurrentGain.SaveData(oXml, "CurrentGain")

                oXml.AddChildElement("FieldPairs")
                oXml.IntoElem()
                Dim doPair As ReceptiveFieldPair
                Dim aryRemovePairs As New ArrayList
                For Each deEntry As DictionaryEntry In m_aryReceptiveFieldPairs
                    doPair = DirectCast(deEntry.Value, ReceptiveFieldPair)

                    If doPair.IsValidPair Then
                        doPair.SaveData(dsSim, doStructure, oXml)
                    Else
                        aryRemovePairs.Add(doPair)
                    End If
                Next
                oXml.OutOfElem()

                oXml.OutOfElem()

                For Each doPair In aryRemovePairs
                    m_aryReceptiveFieldPairs.Remove(doPair.ID)
                Next
            End If

            If Me Is doStructure.RootBody Then
                oXml.AddChildElement("Freeze", m_bFreeze)
            End If

            If Not m_JointToParent Is Nothing Then
                m_JointToParent.SaveData(dsSim, doStructure, oXml)
            End If

            If m_aryChildBodies.Count > 0 Then
                oXml.AddChildElement("ChildBodies")
                oXml.IntoElem()

                Dim doChild As AnimatTools.DataObjects.Physical.RigidBody
                For Each deEntry As DictionaryEntry In m_aryChildBodies
                    doChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
                    doChild.SaveData(dsSim, doStructure, oXml)
                Next

                oXml.OutOfElem() 'Outof ChildBodies Element
            End If

            If m_aryOdorSources.Count > 0 Then
                oXml.AddChildElement("OdorSources")
                oXml.IntoElem()

                Dim doOdor As AnimatTools.DataObjects.Physical.Odor
                For Each deEntry As DictionaryEntry In m_aryOdorSources
                    doOdor = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.Odor)
                    doOdor.SaveData(dsSim, oXml)
                Next

                oXml.OutOfElem() 'Outof ChildBodies Element
            End If

            oXml.AddChildElement("FoodSource", m_bFoodSource)
            If m_bFoodSource Then
                m_snFoodQuantity.SaveData(oXml, "FoodQuantity")
                m_snMaxFoodQuantity.SaveData(oXml, "MaxFoodQuantity")
                m_snFoodReplenishRate.SaveData(oXml, "FoodReplenishRate")
                m_snFoodEnergyContent.SaveData(oXml, "FoodEnergyContent")
            End If

            oXml.OutOfElem() 'Outof BodyPart Element

        End Sub

        Public Sub GetTransparentBodies(ByRef aryTransparent As ArrayList, ByRef aryNonTransparent As ArrayList, ByVal cmCommand As AnimatTools.Forms.BodyPlan.Command.enumCommandMode)

            If Not Me Is Nothing Then
                If Me.Alpha < 255 Or cmCommand = AnimatTools.Forms.BodyPlan.Command.enumCommandMode.SelectJoints Or cmCommand = Forms.BodyPlan.Command.enumCommandMode.SelectReceptiveFields Then
                    aryTransparent.Add(Me)
                    Me.CalculateDistanceFromCamera()

                Else
                    aryNonTransparent.Add(Me)
                End If

                Dim dbChild As AnimatTools.DataObjects.Physical.RigidBody
                For Each deEntry As DictionaryEntry In m_aryChildBodies
                    dbChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
                    dbChild.GetTransparentBodies(aryTransparent, aryNonTransparent, cmCommand)
                Next

                If Not Me.JointToParent Is Nothing Then
                    'If Not cmCommand = AnimatTools.Forms.BodyPlan.Command.enumCommandMode.SelectJoints AndAlso Not cmCommand = Forms.BodyPlan.Command.enumCommandMode.SelectReceptiveFields Then
                    '    aryTransparent.Add(JointToParent)
                    'Else
                    aryNonTransparent.Add(JointToParent)
                    'End If
                End If
            End If
        End Sub

        Protected Overridable Sub CalculateDistanceFromCamera()

        End Sub

        Public Overrides Sub Draw(ByVal cmCommand As AnimatTools.Forms.BodyPlan.Command.enumCommandMode)

            'Dim dbChild As AnimatTools.DataObjects.Physical.RigidBody
            'For Each deEntry As DictionaryEntry In m_aryChildBodies
            '    dbChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
            '    dbChild.Draw(cmCommand)
            'Next

        End Sub

        'Public Overridable Sub DrawJoint(ByVal cmCommand As AnimatTools.Forms.BodyPlan.Command.enumCommandMode)  'DX dependent

        '    If Not m_JointToParent Is Nothing Then m_JointToParent.Draw(cmCommand)

        '    Dim dbChild As AnimatTools.DataObjects.Physical.RigidBody
        '    For Each deEntry As DictionaryEntry In m_aryChildBodies
        '        dbChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
        '        dbChild.DrawJoint(cmCommand)
        '    Next
        'End Sub

        Public Overridable Sub TestIntersection(ByVal x As Integer, ByVal y As Integer, ByVal cmCommand As AnimatTools.Forms.BodyPlan.Command.enumCommandMode, ByRef colSelected As ArrayList)

            If Not m_JointToParent Is Nothing Then m_JointToParent.TestIntersection(x, y, cmCommand, colSelected)

            Dim dbChild As AnimatTools.DataObjects.Physical.RigidBody
            For Each deEntry As DictionaryEntry In m_aryChildBodies
                dbChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
                dbChild.TestIntersection(x, y, cmCommand, colSelected)
            Next
        End Sub

        Public Overridable Sub GetBodyExtent(ByRef iMaxX As Single, ByRef iMinX As Single, ByRef iMaxY As Single, ByRef iMinY As Single)
            Dim dbChild As AnimatTools.DataObjects.Physical.RigidBody
            For Each deEntry As DictionaryEntry In m_aryChildBodies
                dbChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
                dbChild.GetBodyExtent(iMaxX, iMinX, iMaxY, iMinY)
            Next
        End Sub

        Public Overridable Sub CalculateBodyExtent(ByRef iMaxX As Single, ByRef iMinX As Single, ByRef iMaxY As Single, ByRef iMinY As Single)
        End Sub

        Public Overridable Sub AddChildBody(ByVal rbChildBody As AnimatTools.DataObjects.Physical.RigidBody)

            rbChildBody.IsRoot = False

            If Not Me.ChildBodies.Contains(rbChildBody.ID) Then
                Me.ChildBodies.Add(rbChildBody.ID, rbChildBody)
            End If

            Dim doStruct As AnimatTools.DataObjects.Physical.PhysicalStructure = Me.ParentStructure
            If Not rbChildBody Is Nothing AndAlso Not doStruct.BodyEditor Is Nothing AndAlso Not doStruct.BodyEditor.HierarchyBar Is Nothing Then
                rbChildBody.CreateBodyPlanTreeView(Util.Simulation, doStruct, Me, doStruct.BodyEditor.HierarchyBar)
            End If

            Me.ManualAddHistory(New AnimatTools.Framework.UndoSystem.AddBodyPartEvent(doStruct.BodyEditor, doStruct, Me, rbChildBody))
        End Sub

        Public Overridable Sub GetChildPartsList(ByVal aryParts As AnimatTools.Collections.SortedBodyParts)

            aryParts.Add(Me.ID, Me)

            If Not m_JointToParent Is Nothing Then
                aryParts.Add(m_JointToParent.ID, m_JointToParent)
            End If

            Dim doChild As AnimatTools.DataObjects.Physical.RigidBody
            For Each deEntry As DictionaryEntry In m_aryChildBodies
                doChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
                doChild.GetChildPartsList(aryParts)
            Next

        End Sub

        Public Overridable Sub GetChildBodiesList(ByVal aryBodies As AnimatTools.Collections.SortedRigidBodies)

            aryBodies.Add(Me.ID, Me)

            Dim doChild As AnimatTools.DataObjects.Physical.RigidBody
            For Each deEntry As DictionaryEntry In m_aryChildBodies
                doChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
                doChild.GetChildBodiesList(aryBodies)
            Next

        End Sub

        Public Overridable Sub GetChildJointsList(ByVal aryJoints As AnimatTools.Collections.SortedJoints)

            If Not m_JointToParent Is Nothing Then
                aryJoints.Add(m_JointToParent.ID, m_JointToParent)
            End If

            Dim doChild As AnimatTools.DataObjects.Physical.RigidBody
            For Each deEntry As DictionaryEntry In m_aryChildBodies
                doChild = DirectCast(deEntry.Value, AnimatTools.DataObjects.Physical.RigidBody)
                doChild.GetChildJointsList(aryJoints)
            Next

        End Sub

        Public Overrides Sub UnitsChanged(ByVal ePrevMass As AnimatTools.DataObjects.Physical.Environment.enumMassUnits, _
                                          ByVal eNewMass As AnimatTools.DataObjects.Physical.Environment.enumMassUnits, _
                                          ByVal fltMassChange As Single, _
                                          ByVal ePrevDistance As AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits, _
                                          ByVal eNewDistance As AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits, _
                                          ByVal fltDistanceChange As Single)

            Dim iDistDiff As Integer = CInt(Util.Environment.DisplayDistanceUnits) - CInt(Util.Environment.DisplayDistanceUnits(ePrevDistance))
            Dim fltDensityDistChange As Single = CSng(10 ^ iDistDiff)

            Dim fltValue As Double = (m_snDensity.ActualValue / Math.Pow(10, CInt(ePrevMass))) * (Math.Pow(fltDensityDistChange, 3) / fltMassChange)
            Dim eSCale As ScaledNumber.enumNumericScale = CType(Util.Environment.MassUnits, ScaledNumber.enumNumericScale)
            Dim strUnits As String = "g/" & Util.Environment.DistanceUnitAbbreviation(Util.Environment.DisplayDistanceUnits) & "^3"
            m_snDensity = New ScaledNumber(Me, "Density", fltValue, eSCale, strUnits, strUnits)

            m_snReceptiveFieldDistance.ActualValue = m_snReceptiveFieldDistance.ActualValue * fltDistanceChange

            If Not m_JointToParent Is Nothing Then
                m_JointToParent.UnitsChanged(ePrevMass, eNewMass, fltMassChange, ePrevDistance, eNewDistance, fltDistanceChange)
            End If

            Dim doChild As RigidBody
            For Each deEntry As DictionaryEntry In m_aryChildBodies
                doChild = DirectCast(deEntry.Value, RigidBody)
                doChild.UnitsChanged(ePrevMass, eNewMass, fltMassChange, ePrevDistance, eNewDistance, fltDistanceChange)
            Next
        End Sub

        Protected Overridable Function IncludeReceptiveFieldPairsInModule(ByVal nmNeuralModule As DataObjects.Behavior.NeuralModule) As Boolean

            Dim doOrganism As DataObjects.Physical.Organism

            If Not Me.ParentStructure Is Nothing AndAlso TypeOf Me.ParentStructure Is DataObjects.Physical.Organism Then
                doOrganism = DirectCast(Me.ParentStructure, DataObjects.Physical.Organism)

                Dim doPair As ReceptiveFieldPair
                For Each deEntry As DictionaryEntry In m_aryReceptiveFieldPairs
                    doPair = DirectCast(deEntry.Value, ReceptiveFieldPair)

                    If nmNeuralModule.GetType Is doPair.Neuron.NeuralModuleType Then
                        If doOrganism.BehavioralNodes.Contains(doPair.Neuron.ID) Then
                            Return True
                        End If
                    End If
                Next
            End If

            Return False
        End Function

        Public Overridable Sub AddContactAdapters(ByVal nmPhysicsModule As DataObjects.Behavior.NeuralModule, ByVal m_aryNodes As Collections.SortedNodes)
            Dim doOrganism As DataObjects.Physical.Organism

            If Not Me.ParentStructure Is Nothing AndAlso TypeOf Me.ParentStructure Is DataObjects.Physical.Organism AndAlso m_aryReceptiveFieldPairs.Count > 0 Then
                doOrganism = DirectCast(Me.ParentStructure, DataObjects.Physical.Organism)

                'For each rigid body we could have one contact adapter for each neural module. Basically we need to divide out the associated neurons
                'so that they have a target neural module ID the same as their native module. This will ensure that those neurons are updated at the correct
                'times during the simulation. So first we make an array of adapters for each neural module, and then as we loop through the association pairs
                'we add them to the adapter associated with a given neural module and then only save the adapters that end up with neuron pairs in them.
                Dim aryAdapters As New ArrayList
                Dim doAdapter As DataObjects.Behavior.Nodes.ContactAdapter
                For Each deEntry As DictionaryEntry In doOrganism.NeuralModules
                    doAdapter = New DataObjects.Behavior.Nodes.ContactAdapter(nmPhysicsModule)
                    doAdapter.RigidBody = Me
                    doAdapter.TargetNeuralModule = DirectCast(deEntry.Value, DataObjects.Behavior.NeuralModule)
                    aryAdapters.Add(doAdapter)
                Next

                Dim doPair As ReceptiveFieldPair
                For Each deRFEntry As DictionaryEntry In m_aryReceptiveFieldPairs
                    doPair = DirectCast(deRFEntry.Value, ReceptiveFieldPair)

                    If doOrganism.BehavioralNodes.Contains(doPair.Neuron.ID) Then
                        For Each doAdapter In aryAdapters
                            If doAdapter.TargetNeuralModule.GetType() Is doPair.Neuron.NeuralModuleType Then
                                doAdapter.ReceptiveFieldPairs.Add(doPair)
                            End If
                        Next
                    End If
                Next

                For Each doAdapter In aryAdapters
                    If doAdapter.ReceptiveFieldPairs.Count > 0 Then
                        m_aryNodes.Add(doAdapter.ID, doAdapter)
                    End If
                Next
            End If

            'dwc change
            'Now add the contact adapters for any children objects.
            Dim doChild As RigidBody
            For Each deEntry As DictionaryEntry In m_aryChildBodies
                doChild = DirectCast(deEntry.Value, RigidBody)
                doChild.AddContactAdapters(nmPhysicsModule, m_aryNodes)
            Next

        End Sub

        Public Overridable Function FindReceptiveField(ByVal fltX As Single, ByVal fltY As Single, ByVal fltZ As Single, ByRef iIndex As Integer) As Boolean
            iIndex = m_aryReceptiveFields.BinarySearch(New AnimatTools.Framework.Vec3d(Nothing, fltX, fltY, fltZ))

            If (iIndex < 0) Then
                Return False
            Else
                Return True
            End If
        End Function


        Public Overridable Sub DumpReceptiveFields()
            Dim i As Integer = 0
            For Each vField As AnimatTools.Framework.Vec3d In m_aryReceptiveFields
                Debug.WriteLine("Index: " + i.ToString() + "  (" + vField.X.ToString() + ", " + vField.Y.ToString() + ", " + vField.Z.ToString() + ")")
                i = i + 1
            Next
        End Sub

        Public Overridable Sub SortReceptiveFields()
            'I need to sort the list of receptive fields, and I need to remove any duplicates
            m_aryReceptiveFields.Sort()

            Dim aryFields As New ArrayList

            For Each vField As Vec3d In m_aryReceptiveFields
                'Only add in vectors that have not already been added
                If aryFields.BinarySearch(vField) <> 0 Then
                    aryFields.Add(vField)
                End If
            Next

            'Now reset the receptive fields array to be this new array
            m_aryReceptiveFields = aryFields
        End Sub

        'If the receptive field vertices are changed then we need to go back through the list of field pairs and find the vertex that is closest to
        'each of the vertices in the pairs.
        Protected Overridable Sub VerifyReceptiveFielPairs()

            Dim doPair As ReceptiveFieldPair
            For Each deEntry As DictionaryEntry In Me.ReceptiveFieldPairs
                doPair = DirectCast(deEntry.Value, ReceptiveFieldPair)
                doPair.Vertex = FindClosestVertex(doPair.Vertex)
            Next
        End Sub

        Protected Overridable Function FindClosestVertex(ByVal vOrig As Vec3d) As Vec3d
            Dim fltMin As Double = -1
            Dim fltDist As Double
            Dim vMin As Vec3d

            For Each vVertex As Vec3d In Me.ReceptiveFields
                fltDist = Util.Distance(vOrig, vVertex)
                If fltMin = -1 OrElse fltDist < fltMin Then
                    fltMin = fltDist
                    vMin = vVertex
                End If
            Next

            Return vMin
        End Function

#End Region

#Region " Events "


#End Region

    End Class

End Namespace
