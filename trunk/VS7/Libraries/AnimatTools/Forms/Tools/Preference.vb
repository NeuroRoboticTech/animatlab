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

Namespace Forms.Tools

    Public Class Preference
        Inherits System.Windows.Forms.Form

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
        Friend WithEvents ctrlTabs As System.Windows.Forms.TabControl
        Friend WithEvents tbAutoUpdate As System.Windows.Forms.TabPage
        Friend WithEvents tbTest As System.Windows.Forms.TabPage
        Friend WithEvents Label1 As System.Windows.Forms.Label
        Friend WithEvents cbUpdateInterval As System.Windows.Forms.ComboBox
        Friend WithEvents bt_OKButton As System.Windows.Forms.Button
        Friend WithEvents bt_CancelButton As System.Windows.Forms.Button
        Friend WithEvents btnCheckForUpdates As System.Windows.Forms.Button
        <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
            Me.bt_OKButton = New System.Windows.Forms.Button
            Me.ctrlTabs = New System.Windows.Forms.TabControl
            Me.tbAutoUpdate = New System.Windows.Forms.TabPage
            Me.cbUpdateInterval = New System.Windows.Forms.ComboBox
            Me.Label1 = New System.Windows.Forms.Label
            Me.tbTest = New System.Windows.Forms.TabPage
            Me.bt_CancelButton = New System.Windows.Forms.Button
            Me.btnCheckForUpdates = New System.Windows.Forms.Button
            Me.ctrlTabs.SuspendLayout()
            Me.tbAutoUpdate.SuspendLayout()
            Me.SuspendLayout()
            '
            'bt_OKButton
            '
            Me.bt_OKButton.Location = New System.Drawing.Point(8, 240)
            Me.bt_OKButton.Name = "bt_OKButton"
            Me.bt_OKButton.Size = New System.Drawing.Size(48, 24)
            Me.bt_OKButton.TabIndex = 1
            Me.bt_OKButton.Text = "OK"
            '
            'ctrlTabs
            '
            Me.ctrlTabs.Controls.Add(Me.tbAutoUpdate)
            Me.ctrlTabs.Controls.Add(Me.tbTest)
            Me.ctrlTabs.Location = New System.Drawing.Point(8, 8)
            Me.ctrlTabs.Name = "ctrlTabs"
            Me.ctrlTabs.SelectedIndex = 0
            Me.ctrlTabs.Size = New System.Drawing.Size(272, 224)
            Me.ctrlTabs.TabIndex = 2
            '
            'tbAutoUpdate
            '
            Me.tbAutoUpdate.Controls.Add(Me.btnCheckForUpdates)
            Me.tbAutoUpdate.Controls.Add(Me.cbUpdateInterval)
            Me.tbAutoUpdate.Controls.Add(Me.Label1)
            Me.tbAutoUpdate.Location = New System.Drawing.Point(4, 22)
            Me.tbAutoUpdate.Name = "tbAutoUpdate"
            Me.tbAutoUpdate.Size = New System.Drawing.Size(264, 198)
            Me.tbAutoUpdate.TabIndex = 0
            Me.tbAutoUpdate.Text = "Auto Update"
            '
            'cbUpdateInterval
            '
            Me.cbUpdateInterval.Items.AddRange(New Object() {"Never", "Daily", "Weekly", "Monthly"})
            Me.cbUpdateInterval.Location = New System.Drawing.Point(16, 48)
            Me.cbUpdateInterval.Name = "cbUpdateInterval"
            Me.cbUpdateInterval.Size = New System.Drawing.Size(208, 21)
            Me.cbUpdateInterval.TabIndex = 1
            '
            'Label1
            '
            Me.Label1.Location = New System.Drawing.Point(16, 24)
            Me.Label1.Name = "Label1"
            Me.Label1.Size = New System.Drawing.Size(240, 16)
            Me.Label1.TabIndex = 0
            Me.Label1.Text = "how often would you like to run the update"
            '
            'tbTest
            '
            Me.tbTest.Location = New System.Drawing.Point(4, 22)
            Me.tbTest.Name = "tbTest"
            Me.tbTest.Size = New System.Drawing.Size(264, 198)
            Me.tbTest.TabIndex = 1
            Me.tbTest.Text = "Test Tab"
            '
            'bt_CancelButton
            '
            Me.bt_CancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel
            Me.bt_CancelButton.Location = New System.Drawing.Point(72, 240)
            Me.bt_CancelButton.Name = "bt_CancelButton"
            Me.bt_CancelButton.Size = New System.Drawing.Size(48, 24)
            Me.bt_CancelButton.TabIndex = 3
            Me.bt_CancelButton.Text = "Cancel"
            '
            'btnCheckForUpdates
            '
            Me.btnCheckForUpdates.Location = New System.Drawing.Point(16, 80)
            Me.btnCheckForUpdates.Name = "btnCheckForUpdates"
            Me.btnCheckForUpdates.Size = New System.Drawing.Size(112, 24)
            Me.btnCheckForUpdates.TabIndex = 2
            Me.btnCheckForUpdates.Text = "Check for Updates"
            '
            'Preference
            '
            Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
            Me.ClientSize = New System.Drawing.Size(292, 273)
            Me.Controls.Add(Me.bt_CancelButton)
            Me.Controls.Add(Me.ctrlTabs)
            Me.Controls.Add(Me.bt_OKButton)
            Me.Name = "Preference"
            Me.Text = "Form1"
            Me.ctrlTabs.ResumeLayout(False)
            Me.tbAutoUpdate.ResumeLayout(False)
            Me.ResumeLayout(False)

        End Sub

#End Region

        Private Sub bt_OKButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles bt_OKButton.Click

            'Dim File
            'Dim iFileNo As Integer
            'Dim objWriter As New IO.StreamWriter(File.Open("", FileMode.open))

            'Util.Application.AutoUpdateInterval() '= AnimatApplication.enumAutoUpdateInterval
            'Dim temp As cbUpdateInterval.SelectedIndex
            Util.Application.AutoUpdateInterval = CType(cbUpdateInterval.SelectedIndex, AnimatTools.Forms.AnimatApplication.enumAutoUpdateInterval)
            Util.UpdateConfigFile(Util.Application.AutoUpdateInterval.ToString(), Util.Application.LastAutoUpdateTime)
            Me.Close()
        End Sub

        Private Sub tbAutoUpdate_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles tbAutoUpdate.Click

        End Sub

        Private Sub cbUpdateInterval_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cbUpdateInterval.SelectedIndexChanged
            'cbUpdateInterval.SelectedIndex()
        End Sub
        Protected Overrides Sub OnLoad(ByVal e As System.EventArgs)
            Try
                cbUpdateInterval.SelectedIndex = Util.Application.AutoUpdateInterval
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub


        ' Protected Overrides Sub OnLoad(ByVal e As System.EventArgs)
        '     cbUpdateInterval.SelectedIndex = Util.Application.AutoUpdateInterval
        '     Try
        '         Dim oXml As New AnimatTools.Interfaces.StdXml
        ''         oXml.Load(MyBase.ApplicationDirectory() & "AnimatLab.config")
        '          If oXml.FindElement("updateFrequency", True) Then
        '
        '         End If
        '      Catch ex As System.Exception
        '        AnimatTools.Framework.Util.DisplayError(ex)
        '      End Try

        'End Sub

        Private Sub btnCheckForUpdates_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnCheckForUpdates.Click
            Try
                Util.Application.CheckForUpdates(True)
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
                Util.DisplayError(ex)
            End Try

        End Sub

    End Class

End Namespace
