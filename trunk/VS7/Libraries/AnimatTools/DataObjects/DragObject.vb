Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports Crownwood.Magic.Common
Imports Crownwood.Magic.Controls
Imports Crownwood.Magic.Docking
Imports Crownwood.Magic.Menus
Imports AnimatTools.Framework

Namespace DataObjects

    Public MustInherit Class DragObject
        Inherits AnimatTools.Framework.DataObject

#Region " Attributes "

        Protected m_Image As System.Drawing.Image
        Protected m_DragImage As System.Drawing.Image

        Protected m_thDataTypes As New TypeHelpers.DataTypeID(Me)
        Protected m_thIncomingDataType As AnimatTools.DataObjects.DataType

#End Region

#Region " Properties "

        Public MustOverride Property ItemName() As String
        Public MustOverride ReadOnly Property ImageName() As String


        <Browsable(False)> _
        Public Overridable Property Image() As System.Drawing.Image
            Get
                If m_Image Is Nothing Then
                    Dim myAssembly As System.Reflection.Assembly
                    myAssembly = System.Reflection.Assembly.Load(Me.AssemblyModuleName)
                    m_Image = ImageManager.LoadImage(myAssembly, Me.ImageName)
                End If

                Return m_Image
            End Get
            Set(ByVal Value As System.Drawing.Image)
                m_Image = Value
            End Set
        End Property

        Public MustOverride ReadOnly Property DragImageName() As String

        <Browsable(False)> _
        Public Overridable Property DragImage() As System.Drawing.Image
            Get
                If m_DragImage Is Nothing AndAlso Not m_Image Is Nothing Then
                    Return m_Image
                Else
                    Return m_DragImage
                End If
            End Get
            Set(ByVal Value As System.Drawing.Image)
                m_DragImage = Value
            End Set
        End Property

        Public MustOverride ReadOnly Property DataColumnModuleName() As String
        Public MustOverride ReadOnly Property DataColumnClassType() As String
        Public MustOverride ReadOnly Property StructureID() As String
        Public MustOverride ReadOnly Property CanBeCharted() As Boolean

        <Browsable(False)> _
        Public Overridable ReadOnly Property CompatibleStimuli() As Collections.Stimuli
            Get
                Dim aryStims As New Collections.Stimuli(Me)

                'Lets generate a list of the compatibleStimuli. We need to look through the list of stimuli in the application
                'object and only pick the ones that have this in their list of compatibledataobjects.
                For Each doStim As ExternalStimuli.Stimulus In Util.Application.ExternalStimuli
                    If doStim.CompatibleDataObjects.Contains(Me.GetType.FullName) Then
                        aryStims.Add(doStim)
                    End If
                Next

                Return aryStims
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property AllowStimulus() As Boolean
            Get
                Return True
            End Get
        End Property

        'For some objects, like body parts, what we really want to chart is not the behavior.bodypart object, but the underlying linked object.
        'This gives the object a way to return a reference to someing else to be charted instead of itself if this is needed.
        <Browsable(False)> _
        Public Overridable ReadOnly Property DataColumnItem() As DragObject
            Get
                Return Me
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable Property DataTypes() As TypeHelpers.DataTypeID
            Get
                Return m_thDataTypes
            End Get
            Set(ByVal Value As TypeHelpers.DataTypeID)
                If Not Value Is Nothing Then
                    Try
                        m_thDataTypes.ID = Value.ID
                    Catch ex As System.Exception
                    End Try
                End If
            End Set
        End Property

        Public Overridable ReadOnly Property IncomingDataType() As AnimatTools.DataObjects.DataType
            Get
                Return m_thIncomingDataType
            End Get
        End Property

        Public Overridable ReadOnly Property TimeStep() As Double
            Get
                'Get the default time step of the physics neural module and return that.
                If Not Util.Environment Is Nothing Then
                    Return Util.Environment.PhysicsTimeStep.ActualValue
                End If
            End Get
        End Property

        Public Overridable ReadOnly Property RequiresAutoDataCollectInterval() As Boolean
            Get
                Return False
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)
        End Sub

        Public MustOverride Sub SaveDataColumnToXml(ByRef oXml As AnimatTools.Interfaces.StdXml)
        Public MustOverride Function FindDragObject(ByVal strStructureName As String, ByVal strDataItemID As String, Optional ByVal bThrowError As Boolean = True) As DataObjects.DragObject

        Public Overridable Sub SaveXml(ByRef oXml As Interfaces.StdXml)

        End Sub

        Protected Overrides Sub BuildProperties()

        End Sub

        Public Overridable Sub SelectStimulusType()

        End Sub

        Protected Overridable Sub AddCompatibleStimulusType(ByVal strStimulusType As String)
            AddCompatibleStimulusType(strStimulusType, Me)
        End Sub

        Protected Overridable Sub AddCompatibleStimulusType(ByVal strStimulusType As String, ByVal doDataObject As Framework.DataObject)

            strStimulusType = strStimulusType.ToLower.Trim
            For Each doStim As ExternalStimuli.Stimulus In Util.Application.ExternalStimuli
                'If the control types match then add this object to the list for this stimulus
                If doStim.ControlType.ToLower.Trim = strStimulusType Then
                    doStim.AddCompatibleDataObject(doDataObject)
                End If
            Next

        End Sub

        Protected Overridable Sub RemoveCompatibleStimulus(ByVal strStimulusType As String)

            strStimulusType = strStimulusType.ToLower.Trim
            For Each doStim As ExternalStimuli.Stimulus In Util.Application.ExternalStimuli
                'If the control types match then add this object to the list for this stimulus
                If doStim.ControlType.ToLower.Trim = strStimulusType Then
                    If Not doStim.CompatibleDataObjects Is Nothing AndAlso doStim.CompatibleDataObjects.Contains(Me.GetType.FullName) Then
                        doStim.CompatibleDataObjects.Remove(Me.GetType.FullName)
                    End If
                End If
            Next

        End Sub

        Public Overridable Function CreateDataItemTreeView(ByVal frmDataItem As Forms.Tools.SelectDataItem, ByVal tnParent As TreeNode, ByVal tpTemplatePartType As Type) As TreeNode

            'If a template part type is supplied and this part is not one of those template types then do not add it to the tree view
            If tpTemplatePartType Is Nothing OrElse (Not tpTemplatePartType Is Nothing AndAlso Util.IsTypeOf(Me.GetType(), tpTemplatePartType, False)) Then
                Dim myAssembly As System.Reflection.Assembly
                myAssembly = System.Reflection.Assembly.Load(Me.AssemblyModuleName)
                frmDataItem.ImageManager.AddImage(myAssembly, Me.ImageName)

                Dim tnNode As TreeNode = tnParent.Nodes.Add(Me.ItemName)
                tnNode.ImageIndex = frmDataItem.ImageManager.GetImageIndex(Me.ImageName)
                tnNode.SelectedImageIndex = frmDataItem.ImageManager.GetImageIndex(Me.ImageName)
                tnNode.Tag = Me

                Return tnNode
            Else
                Return Nothing
            End If
        End Function

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)
        End Sub

#End Region

    End Class

End Namespace
