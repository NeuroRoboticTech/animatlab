Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports Crownwood.Magic.Controls
Imports AnimatTools.Framework

Namespace DataObjects.Behavior.Nodes

    Public MustInherit Class Adapter
        Inherits Behavior.Node

#Region " Attributes "

        Protected m_bnOrigin As Behavior.Node
        Protected m_bnDestination As Behavior.Node

        Protected m_gnGain As AnimatTools.DataObjects.Gain

        'Only used during the loading process.
        Protected m_strOriginID As String
        Protected m_strDestinationID As String
        Protected m_strDataTypeID As String

#End Region

#Region " Properties "

        Public Overrides ReadOnly Property TypeName() As String
            Get
                Return "Standard Adapter"
            End Get
        End Property

        Public Overrides ReadOnly Property IsDisplayedInIconPanel() As Boolean
            Get
                Return False
            End Get
        End Property

        Public Overridable Property Gain() As AnimatTools.DataObjects.Gain
            Get
                Return m_gnGain
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Gain)
                If Not m_gnGain Is Nothing Then m_gnGain.ParentData = Nothing
                m_gnGain = Value
                If Not m_gnGain Is Nothing Then
                    m_gnGain.ParentData = Me
                    m_gnGain.GainPropertyName = "Gain"
                End If
            End Set
        End Property

        Public Overridable ReadOnly Property Origin() As Behavior.Node
            Get
                Return m_bnOrigin
            End Get
        End Property

        Public Overridable ReadOnly Property Destination() As Behavior.Node
            Get
                Return m_bnDestination
            End Get
        End Property

        Public Overrides Property DataTypes() As TypeHelpers.DataTypeID
            Get
                Return m_thDataTypes
            End Get
            Set(ByVal Value As TypeHelpers.DataTypeID)
                If Not Value Is Nothing Then
                    m_thDataTypes = Value
                    CheckForErrors()
                    SetGainLimits()
                End If
            End Set
        End Property

        Public Overrides ReadOnly Property IncomingDataType() As AnimatTools.DataObjects.DataType
            Get
                If Not m_bnDestination Is Nothing Then
                    Return m_bnDestination.IncomingDataType
                End If
            End Get
        End Property

        Public Overrides ReadOnly Property NeuralModuleType() As System.Type
            Get
                Return GetType(AnimatTools.DataObjects.Behavior.PhysicsModule)
            End Get
        End Property

        Public MustOverride ReadOnly Property AdapterType() As String

        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return "AnimatTools.StandardAdapter.gif"
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property CanBeCharted() As Boolean
            Get
                Return False
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property AllowStimulus() As Boolean
            Get
                Return False
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)

            Try

                Shape = Behavior.Node.enumShape.Display
                Size = New SizeF(40, 40)
                Me.DrawColor = Color.Black
                Me.FillColor = Color.Salmon
                m_bEnabled = True

                Dim myAssembly As System.Reflection.Assembly
                myAssembly = System.Reflection.Assembly.Load("AnimatTools")

                Me.Image = AnimatTools.Framework.ImageManager.LoadImage(myAssembly, "AnimatTools.StandardAdapter.gif")
                Me.Name = "Standard Adapter"

                Me.Font = New Font("Arial", 12, FontStyle.Bold)
                Me.Description = "Provides an interface adapter between nodes and the physics engine."

                AddCompatibleLink(New Behavior.Links.Adapter(Nothing))

                m_gnGain = New AnimatTools.DataObjects.Gains.Polynomial(Me, "Gain", "Input Variable", "Output Variable", False, False)

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim bnOrig As Adapter = DirectCast(doOriginal, Adapter)

            m_gnGain = DirectCast(bnOrig.m_gnGain.Clone(Me, bCutData, doRoot), AnimatTools.DataObjects.Gain)
            m_bEnabled = bnOrig.m_bEnabled
        End Sub

        Public Overrides Sub BeforeAddLink(ByVal blLink As Link)

            If blLink.ActualDestination Is Me AndAlso Me.InLinks.Count > 0 Then
                Throw New System.Exception("You can only have one incoming link to an adapter node!")
            End If

            If blLink.ActualOrigin Is Me AndAlso Me.OutLinks.Count > 0 Then
                Throw New System.Exception("You can only have one outgoing link from an adapter node!")
            End If

        End Sub

        Public Overrides Sub AfterAddLink(ByVal blLink As Link)

            'If this is the destination then get the origin from the other end
            If blLink.ActualDestination Is Me Then
                m_bnOrigin = blLink.Origin
                Me.m_thDataTypes = DirectCast(m_bnOrigin.DataTypes.Clone(Me, False, Nothing), TypeHelpers.DataTypeID)
                SetGainLimits()
            End If

            'If this is the Origin then get the destination from the other end
            If blLink.ActualOrigin Is Me Then
                m_bnDestination = blLink.Destination
            End If

            MyBase.AfterAddLink(blLink)
        End Sub

        Protected Sub SetGainLimits()
            If Not m_gnGain Is Nothing AndAlso Not m_thDataTypes Is Nothing AndAlso Not m_thDataTypes.Value Is Nothing Then
                m_gnGain.LowerLimit = New ScaledNumber(m_gnGain, "LowerLimit", m_thDataTypes.Value.LowerLimit, _
                                                       m_thDataTypes.Value.LowerLimitscale, _
                                                       m_thDataTypes.Value.Units, _
                                                       m_thDataTypes.Value.UnitsAbbreviation)
                m_gnGain.UpperLimit = New ScaledNumber(m_gnGain, "UpperLimit", m_thDataTypes.Value.UpperLimit, _
                                                       m_thDataTypes.Value.UpperLimitscale, _
                                                       m_thDataTypes.Value.Units, _
                                                       m_thDataTypes.Value.UnitsAbbreviation)
            End If
        End Sub

        Public Overrides Sub BeforeCopy()

            Dim blLink As AnimatTools.DataObjects.Behavior.Link
            For Each deEntry As DictionaryEntry In m_aryLinks
                blLink = DirectCast(deEntry.Value, AnimatTools.DataObjects.Behavior.Link)
                If Not m_ParentDiagram.IsItemSelected(blLink) Then
                    m_ParentDiagram.SelectDataItem(blLink, False)
                End If
            Next

        End Sub

        Public Overrides Sub CreateDiagramDropDownTree(ByVal tvTree As TreeView, ByVal tnParent As TreeNode)
        End Sub


        Public Overrides Sub CheckForErrors()
            MyBase.CheckForErrors()

            If m_ParentEditor Is Nothing OrElse m_ParentEditor.ErrorsBar Is Nothing Then Return

            If m_thDataTypes Is Nothing OrElse m_thDataTypes.ID Is Nothing OrElse m_thDataTypes.ID.Trim.Length = 0 Then
                If Not m_ParentEditor.ErrorsBar.Errors.Contains(DiagramErrors.DataError.GenerateID(Me, DiagramError.enumErrorTypes.DataTypeNotSet)) Then
                    Dim deError As New DiagramErrors.DataError(Me, DiagramError.enumErrorLevel.Error, DiagramError.enumErrorTypes.DataTypeNotSet, _
                                                               "The adapter '" & Me.Text & "' does not have a defined data type pointer value.")
                    m_ParentEditor.ErrorsBar.Errors.Add(deError.ID, deError)
                End If
            Else
                If m_ParentEditor.ErrorsBar.Errors.Contains(DiagramErrors.DataError.GenerateID(Me, DiagramError.enumErrorTypes.DataTypeNotSet)) Then
                    m_ParentEditor.ErrorsBar.Errors.Remove(DiagramErrors.DataError.GenerateID(Me, DiagramError.enumErrorTypes.DataTypeNotSet))
                End If
            End If

        End Sub

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Gain", GetType(AnimatTools.DataObjects.Gain), "Gain", _
                                        "Adapter Properties", "Sets the gain that controls the input/output relationship " & _
                                        "between the two selected items.", m_gnGain, _
                                        GetType(AnimatTools.TypeHelpers.GainTypeEditor), _
                                        GetType(AnimatTools.TypeHelpers.GainTypeConverter)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Source Data Type ID", GetType(AnimatTools.TypeHelpers.DataTypeID), "DataTypes", _
                                        "Adapter Properties", "Sets the type of data to use as an input from the source node into the gain function.", m_thDataTypes, _
                                        GetType(AnimatTools.TypeHelpers.DropDownListEditor), _
                                        GetType(AnimatTools.TypeHelpers.DataTypeIDTypeConverter)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Enabled", GetType(Boolean), "Enabled", _
                                        "Adapter Properties", "Determines if this adapter is enabled or not.", m_bEnabled))

        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()

            If Not m_gnGain Is Nothing Then m_gnGain.ClearIsDirty()
        End Sub

        Public Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.LoadData(oXml)

            oXml.IntoElem()

            m_strOriginID = Util.LoadID(oXml, "Origin", True, "")
            m_strDestinationID = Util.LoadID(oXml, "Destination", True, "")
            m_strDataTypeID = Util.LoadID(oXml, "DataType", True, "")
            m_bEnabled = oXml.GetChildBool("Enabled", True)

            If oXml.FindChildElement("Gain", False) Then
                oXml.IntoChildElement("Gain")
                Dim strAssemblyFile As String = oXml.GetChildString("AssemblyFile")
                Dim strClassName As String = oXml.GetChildString("ClassName")
                oXml.OutOfElem()

                m_gnGain = DirectCast(Util.LoadClass(strAssemblyFile, strClassName, Me), AnimatTools.DataObjects.Gain)
                m_gnGain.LoadData(oXml, "Gain", "Gain")
            End If

            oXml.OutOfElem()

        End Sub

        Public Overrides Sub InitializeAfterLoad(ByVal iAttempt As Integer)

            Try
                MyBase.InitializeAfterLoad(iAttempt)

                If m_bInitialized Then
                    If m_strOriginID.Trim.Length > 0 Then
                        m_bnOrigin = Me.Organism.FindBehavioralNode(m_strOriginID)

                        If Not m_bnOrigin.Initialized Then
                            m_bInitialized = False
                            Return
                        End If

                        m_thDataTypes = DirectCast(m_bnOrigin.DataTypes.Clone(Me, False, Nothing), TypeHelpers.DataTypeID)

                        If Not m_thDataTypes Is Nothing AndAlso m_strDataTypeID.Trim.Length > 0 AndAlso m_strDataTypeID.Trim.Length > 0 Then
                            If Me.m_thDataTypes.DataTypes.Contains(m_strDataTypeID) Then
                                Me.m_thDataTypes.ID = m_strDataTypeID
                            End If
                        End If
                    End If

                    If m_strDestinationID.Trim.Length > 0 Then
                        m_bnDestination = Me.Organism.FindBehavioralNode(m_strDestinationID)
                    End If
                End If

            Catch ex As System.Exception
                m_bInitialized = False
                If iAttempt = 1 Then
                    AnimatTools.Framework.Util.DisplayError(ex)
                End If
            End Try

        End Sub

        Public Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.SaveData(oXml)

            oXml.IntoElem() 'Into Node Element

            If Not m_bnOrigin Is Nothing Then
                oXml.AddChildElement("OriginID", m_bnOrigin.SelectedID)
            Else
                Throw New System.Exception("The orgin node for an adapter has been lost!!")
            End If

            If Not m_bnDestination Is Nothing Then
                oXml.AddChildElement("DestinationID", m_bnDestination.SelectedID)
            Else
                Throw New System.Exception("The destination node for an adapter has been lost!!")
            End If

            If Not m_thDataTypes Is Nothing Then
                oXml.AddChildElement("DataTypeID", m_thDataTypes.ID)
            End If

            If Not m_gnGain Is Nothing Then
                m_gnGain.SaveData(oXml, "Gain")
            End If

            oXml.AddChildElement("Enabled", m_bEnabled)

            oXml.OutOfElem() ' Outof Node Element

        End Sub

#End Region

#End Region

    End Class

End Namespace