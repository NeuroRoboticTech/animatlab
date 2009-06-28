Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports Crownwood.Magic.Controls
Imports System.Text.RegularExpressions
Imports System.Drawing.Design
Imports System.Windows.Forms.Design
Imports AnimatTools.Framework

Namespace TypeHelpers

    Public Class LinkedBodyPartTypeConverter
        Inherits ExpandableObjectConverter

        Public Overloads Overrides Function CanConvertTo(ByVal context As System.ComponentModel.ITypeDescriptorContext, ByVal destinationType As System.Type) As Boolean

            If Util.IsTypeOf(destinationType, GetType(AnimatTools.TypeHelpers.LinkedBodyPart)) Then
                Return True
            End If
            Return MyBase.CanConvertTo(context, destinationType)

        End Function

        Public Overloads Overrides Function ConvertTo(ByVal context As System.ComponentModel.ITypeDescriptorContext, ByVal culture As System.Globalization.CultureInfo, ByVal value As Object, ByVal destinationType As System.Type) As Object

            If Not value Is Nothing AndAlso Util.IsTypeOf(value.GetType, GetType(AnimatTools.TypeHelpers.LinkedBodyPart), False) Then
                Dim thBodyPart As AnimatTools.TypeHelpers.LinkedBodyPart = DirectCast(value, AnimatTools.TypeHelpers.LinkedBodyPart)

                If Not thBodyPart.BodyPart Is Nothing Then
                    Return thBodyPart.BodyPart.Name
                Else
                    Return ""
                End If
            End If

        End Function

    End Class

End Namespace
