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

Namespace TypeHelpers

    Public Class LinkedOdorTypeList
        Inherits TypeHelpers.LinkedOdorType

#Region " Attributes "

#End Region

#Region " Properties "

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)
        End Sub

        Public Sub New(ByVal doOdorType As AnimatTools.DataObjects.Physical.OdorType)
            MyBase.New(doOdorType)
            m_doOdorType = doOdorType
        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim oNew As New LinkedOdorTypeList(doParent)
            oNew.CloneInternal(Me, bCutData, doRoot)
            Return oNew
        End Function

        Protected Overrides Sub BuildProperties()

        End Sub

        Public Overrides Sub BuildPropertyDropDown(ByRef ctrlDropDown As System.Windows.Forms.Control)

            If Not TypeOf (ctrlDropDown) Is ListBox Then
                Throw New System.Exception("The control passed into LinkedOdorType.BuildPropertyDropDown is not a listbox type")
            End If

            Dim lbList As ListBox = DirectCast(ctrlDropDown, ListBox)

            lbList.BeginUpdate()
            lbList.Items.Clear()
            Dim lbSelectedItem As AnimatTools.TypeHelpers.DropDownEntry = Nothing
            Dim doType As DataObjects.Physical.OdorType
            For Each deEntry As DictionaryEntry In Util.Environment.OdorTypes
                doType = DirectCast(deEntry.Value, DataObjects.Physical.OdorType)

                Dim thType As LinkedOdorType = New LinkedOdorTypeList(Me.Parent)
                thType.OdorType = doType
                Dim leItem As New AnimatTools.TypeHelpers.DropDownEntry(doType.Name, thType)

                lbList.Items.Add(leItem)

                If Not m_doOdorType Is Nothing AndAlso m_doOdorType.Name = doType.Name Then
                    lbSelectedItem = leItem
                End If
            Next

            If Not lbSelectedItem Is Nothing Then lbList.SelectedItem = lbSelectedItem
            lbList.DisplayMember = "Display"
            lbList.ValueMember = "Value"

            MyBase.FormatDropDownList(lbList)

            lbList.EndUpdate()
            lbList.Invalidate()

        End Sub

#End Region

    End Class

End Namespace
