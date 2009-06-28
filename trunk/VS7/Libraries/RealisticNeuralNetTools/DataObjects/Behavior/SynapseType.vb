Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.ComponentModel.Design.Serialization
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports Crownwood.Magic.Controls
Imports AnimatTools.Framework

Namespace DataObjects.Behavior

    Public MustInherit Class SynapseType
        Inherits AnimatTools.DataObjects.Behavior.Links.Synapse

#Region " Attributes "

        Protected m_nmNeuralModule As DataObjects.Behavior.NeuralModule

#End Region

#Region " Events "

        Public Event SynapseTypeChanged()

#End Region

#Region " Properties "

        <Browsable(False)> _
        Public Overrides Property Name() As String
            Get
                Return m_strName
            End Get
            Set(ByVal Value As String)
                m_strName = Value

                If Not Me.TreeNode Is Nothing Then
                    Me.TreeNode.Text = m_strName
                End If
            End Set
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property NeuralModuleType() As System.Type
            Get
                Return GetType(RealisticNeuralNetTools.DataObjects.Behavior.NeuralModule)
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides Property ViewSubProperties() As Boolean
            Get
                Return False
            End Get
            Set(ByVal Value As Boolean)
            End Set
        End Property

        <Browsable(False)> _
        Public Property NeuralModule() As DataObjects.Behavior.NeuralModule
            Get
                Return m_nmNeuralModule
            End Get
            Set(ByVal Value As DataObjects.Behavior.NeuralModule)
                m_nmNeuralModule = Value
            End Set
        End Property

        Public MustOverride ReadOnly Property SynapseType() As Integer

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)
        End Sub

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim OrigLink As AnimatTools.DataObjects.Behavior.Data = DirectCast(doOriginal, AnimatTools.DataObjects.Behavior.Data)

            Dim blOrig As SynapseType = DirectCast(OrigLink, SynapseType)

            m_nmNeuralModule = blOrig.m_nmNeuralModule

        End Sub

        Public Overridable Sub SignalSynapseTypeChanged()
            RaiseEvent SynapseTypeChanged()
        End Sub

        Public Overrides Function ToString() As String
            Return Me.Name
        End Function

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Synapse Type", GetType(String), "TypeName", _
                                        "Synapse Properties", "Returns the type of this link.", TypeName(), True))

            Dim pbArrowBag As Crownwood.Magic.Controls.PropertyBag = m_ArrowDst.Properties
            m_Properties.Properties.Add(New PropertySpec("Arrow Destination", pbArrowBag.GetType(), _
                             "ArrowDestination", "Graphical Properties", "Sets the destination arrow shape of the link.", pbArrowBag, _
                              "", GetType(Crownwood.Magic.Controls.ExpandablePropBagConverter)))

            pbArrowBag = m_ArrowMid.Properties
            m_Properties.Properties.Add(New PropertySpec("Arrow Middle", pbArrowBag.GetType(), _
                             "ArrowMiddle", "Graphical Properties", "Sets the middle arrow shape of the link.", pbArrowBag, _
                              "", GetType(Crownwood.Magic.Controls.ExpandablePropBagConverter)))

            pbArrowBag = m_ArrowOrg.Properties
            m_Properties.Properties.Add(New PropertySpec("Arrow Origin", pbArrowBag.GetType(), _
                             "ArrowOrigin", "Graphical Properties", "Sets the origin arrow shape of the link.", pbArrowBag, _
                              "", GetType(Crownwood.Magic.Controls.ExpandablePropBagConverter)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("DashStyle", m_DashStyle.GetType(), "DashStyle", _
                                        "Graphical Properties", "Sets the DashStyle of the pen used to draw the item.", m_DashStyle))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Draw Color", m_clDrawColor.GetType(), "DrawColor", _
                                        "Graphical Properties", "Sets the pen color used to draw the item.", m_clDrawColor))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Draw Width", m_iDrawWidth.GetType(), "DrawWidth", _
                                        "Graphical Properties", "Sets the pen width used to draw the item.", m_iDrawWidth))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Font", m_Font.GetType(), "Font", _
                                        "Graphical Properties", "Sets or returns the font used to display the text associated to the font.", m_Font))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Name", m_strName.GetType(), "Name", _
                                        "Synapse Properties", "Sets the name of this synapse type.", m_strName, _
                                        GetType(AnimatTools.TypeHelpers.MultiLineStringTypeEditor)))

        End Sub

        Public Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            Try
                Me.IsDirty = False

                oXml.IntoElem()  'Into Link Element

                m_strID = Util.LoadID(oXml, "")
                m_strDestinationID = Util.LoadID(oXml, "Destination")
                m_strOriginID = Util.LoadID(oXml, "Origin")
                m_bAdjustDst = oXml.GetChildBool("AdjustDst")
                m_bAdjustOrg = oXml.GetChildBool("AdjustOrg")
                m_ArrowDst.LoadData("ArrowDestination", oXml)
                m_ArrowMid.LoadData("ArrowMiddle", oXml)
                m_ArrowOrg.LoadData("ArrowOrigin", oXml)
                m_eBackMode = DirectCast([Enum].Parse(GetType(enumBackmode), oXml.GetChildString("BackMode"), True), enumBackmode)
                m_DashStyle = DirectCast([Enum].Parse(GetType(System.Drawing.Drawing2D.DashStyle), oXml.GetChildString("DashStyle"), True), System.Drawing.Drawing2D.DashStyle)
                m_clDrawColor.FromArgb(oXml.GetChildInt("DrawColor"))
                m_iDrawWidth = oXml.GetChildInt("DrawWidth")
                m_Font = Util.LoadFont(oXml, "Font")
                m_bHidden = oXml.GetChildBool("Hidden")
                m_eJump = DirectCast([Enum].Parse(GetType(enumJump), oXml.GetChildString("Jump"), True), enumJump)
                m_eLineStyle = DirectCast([Enum].Parse(GetType(enumLineStyle), oXml.GetChildString("LineStyle"), True), enumLineStyle)
                m_bOrthogonalDynamic = oXml.GetChildBool("OrthogonalDynamic")
                m_bOrientedText = oXml.GetChildBool("OrientedText")
                m_bSelectable = oXml.GetChildBool("Selectable")
                m_bStretchable = oXml.GetChildBool("Stretchable")
                m_strText = oXml.GetChildString("Text")
                m_strToolTip = oXml.GetChildString("ToolTip")
                m_strUrl = oXml.GetChildString("Url")
                m_iZOrder = oXml.GetChildInt("ZOrder")

                Me.Name = oXml.GetChildString("Name")

                oXml.OutOfElem()  'Outof Link Element

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)

            Try

                oXml.AddChildElement("Link")
                oXml.IntoElem()  'Into Link Element

                oXml.AddChildElement("AssemblyFile", Me.AssemblyFile)
                oXml.AddChildElement("ClassName", Me.ClassName)

                oXml.AddChildElement("ID", Me.SelectedID)
                oXml.AddChildElement("AdjustDst", m_bAdjustDst)
                oXml.AddChildElement("AdjustOrg", m_bAdjustOrg)
                m_ArrowDst.SaveData("ArrowDestination", oXml)
                m_ArrowMid.SaveData("ArrowMiddle", oXml)
                m_ArrowOrg.SaveData("ArrowOrigin", oXml)
                oXml.AddChildElement("BackMode", m_eBackMode.ToString)
                oXml.AddChildElement("DashStyle", m_DashStyle.ToString)
                oXml.AddChildElement("DrawColor", m_clDrawColor.ToArgb)
                oXml.AddChildElement("DrawWidth", m_iDrawWidth)
                oXml.AddChildElement("DestinationID", "")
                Util.SaveFont(oXml, "Font", m_Font)
                oXml.AddChildElement("Hidden", m_bHidden)
                oXml.AddChildElement("Jump", m_eJump.ToString)
                oXml.AddChildElement("LineStyle", m_eLineStyle.ToString)
                oXml.AddChildElement("OrthogonalDynamic", m_bOrthogonalDynamic)
                oXml.AddChildElement("OriginID", "")
                oXml.AddChildElement("OrientedText", m_bOrientedText)
                oXml.AddChildElement("Selectable", m_bSelectable)
                oXml.AddChildElement("Stretchable", m_bStretchable)
                oXml.AddChildElement("Text", m_strText)
                oXml.AddChildElement("ToolTip", m_strToolTip)
                oXml.AddChildElement("Url", m_strUrl)
                oXml.AddChildElement("ZOrder", m_iZOrder)

                oXml.AddChildElement("Name", Me.Name)

                oXml.OutOfElem()  'Outof Link Element
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overrides Sub InitializeAfterLoad(ByVal iAttempt As Integer)
            m_bInitialized = True
        End Sub

#End Region

#End Region

    End Class

End Namespace

