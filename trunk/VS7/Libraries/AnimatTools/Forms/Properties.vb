Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports Crownwood.Magic.Common
Imports Crownwood.Magic.Controls
Imports Crownwood.Magic.Docking
Imports Crownwood.Magic.Menus
Imports AnimatTools.Framework

Namespace Forms

    Public Class Properties
        Inherits AnimatForm

#Region " Windows Form Designer generated code "

        Public Sub New()
            MyBase.New()

            'This call is required by the Windows Form Designer.
            InitializeComponent()

            'Add any initialization after the InitializeComponent() call

        End Sub

        'Form overrides dispose to clean up the component list.
        Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
            If disposing Then
                If Not (components Is Nothing) Then
                    components.Dispose()
                End If
            End If
            MyBase.Dispose(disposing)
        End Sub

        'Required by the Windows Form Designer
        Private components As System.ComponentModel.IContainer

        'NOTE: The following procedure is required by the Windows Form Designer
        'It can be modified using the Windows Form Designer.  
        'Do not modify it using the code editor.
        Friend WithEvents gridProperty As System.Windows.Forms.PropertyGrid
        <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
            Me.gridProperty = New System.Windows.Forms.PropertyGrid
            Me.SuspendLayout()
            '
            'gridProperty
            '
            Me.gridProperty.CommandsVisibleIfAvailable = True
            Me.gridProperty.Dock = System.Windows.Forms.DockStyle.Fill
            Me.gridProperty.LargeButtons = False
            Me.gridProperty.LineColor = System.Drawing.SystemColors.ScrollBar
            Me.gridProperty.Location = New System.Drawing.Point(0, 0)
            Me.gridProperty.Name = "gridProperty"
            Me.gridProperty.Size = New System.Drawing.Size(292, 266)
            Me.gridProperty.TabIndex = 0
            Me.gridProperty.Text = "Project Properties"
            Me.gridProperty.ViewBackColor = System.Drawing.SystemColors.Window
            Me.gridProperty.ViewForeColor = System.Drawing.SystemColors.WindowText
            '
            'Properties
            '
            Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
            Me.ClientSize = New System.Drawing.Size(292, 266)
            Me.Controls.Add(Me.gridProperty)
            Me.Name = "Properties"
            Me.Text = "Properties"
            Me.ResumeLayout(False)
        End Sub


        'Temporary test code
        Private Sub bag1_GetValue(ByVal sender As Object, ByVal e As PropertySpecEventArgs)
        End Sub

        Private Sub bag1_SetValue(ByVal sender As Object, ByVal e As PropertySpecEventArgs)
        End Sub

#End Region

#Region " Attributes "

        Protected m_PropertyData As Crownwood.Magic.Controls.PropertyBag
        Protected m_PropertyArray() As Crownwood.Magic.Controls.PropertyBag

#End Region

#Region " Properties "

        Public Overrides ReadOnly Property IconName() As String
            Get
                Return "AnimatTools.Properties.gif"
            End Get
        End Property

        Public Property PropertyData() As Crownwood.Magic.Controls.PropertyBag
            Get
                Return m_PropertyData
            End Get
            Set(ByVal Value As Crownwood.Magic.Controls.PropertyBag)

                Try
                    m_PropertyArray = Nothing
                    m_PropertyData = Value

                    If Not m_PropertyData Is Nothing Then
                        Me.gridProperty.SelectedObjects = Nothing
                        Me.gridProperty.SelectedObject = m_PropertyData
                    Else
                        Me.gridProperty.SelectedObjects = Nothing
                        Me.gridProperty.SelectedObject = Nothing
                    End If

                Catch ex As System.Exception
                    AnimatTools.Framework.Util.DisplayError(ex)
                End Try

            End Set
        End Property

        Public Property PropertyArray() As Crownwood.Magic.Controls.PropertyBag()
            Get
                Return m_PropertyArray
            End Get
            Set(ByVal Value As Crownwood.Magic.Controls.PropertyBag())

                Try
                    m_PropertyData = Nothing
                    m_PropertyArray = Value

                    If Not m_PropertyArray Is Nothing Then
                        Me.gridProperty.SelectedObject = Nothing
                        Me.gridProperty.SelectedObjects = m_PropertyArray
                    Else
                        Me.gridProperty.SelectedObject = Nothing
                        Me.gridProperty.SelectedObjects = Nothing
                    End If

                Catch ex As System.Exception
                    AnimatTools.Framework.Util.DisplayError(ex)
                End Try

            End Set
        End Property

#End Region

#Region " Methods "

        Public Overrides Sub Initialize(ByRef frmApplication As AnimatApplication, _
                                          Optional ByVal frmMdiParent As MdiChild = Nothing, _
                                          Optional ByVal frmParent As AnimatForm = Nothing)

            Try
                MyBase.Initialize(frmApplication, frmMdiParent, frmParent)

                If Not m_PropertyData Is Nothing Then
                    Me.gridProperty.SelectedObject = m_PropertyData
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overridable Sub RefreshProperties()
            Me.gridProperty.Refresh()
        End Sub

#End Region

    End Class

End Namespace