Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports Crownwood.Magic.Controls

Namespace DataObjects.Behavior.Links

    Public Class Graphical
        Inherits Behavior.Link

#Region " Attributes "

#End Region

#Region " Properties "

        Public Overrides ReadOnly Property TypeName() As String
            Get
                Return "Graphical Link"
            End Get
        End Property

        Public Overrides ReadOnly Property NeuralModuleType() As System.Type
            Get
                Return Nothing
            End Get
        End Property

        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return "AnimatTools.AdapterLink.gif"
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property CanBeCharted() As Boolean
            Get
                Return False
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)

            m_ArrowDst = New Arrow(Me, Link.enumArrowStyle.None, Link.enumArrowSize.Small, Link.enumArrowAngle.deg15, False)

            Dim myAssembly As System.Reflection.Assembly
            myAssembly = System.Reflection.Assembly.Load("AnimatTools")

            Me.Image = AnimatTools.Framework.ImageManager.LoadImage(myAssembly, "AnimatTools.AdapterLink.gif")
            Me.Name = "Graphcial Link"

            Me.Font = New Font("Arial", 12)
            Me.Description = "Adds a graphical link with no functionality between two nodes."

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim oNewLink As New Behavior.Links.Graphical(doParent)
            oNewLink.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then oNewLink.AfterClone(Me, bCutData, doRoot, oNewLink)
            Return oNewLink
        End Function

#Region " DataObject Methods "

#End Region

#End Region

    End Class

End Namespace

