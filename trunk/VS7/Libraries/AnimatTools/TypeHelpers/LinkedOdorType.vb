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

    Public MustInherit Class LinkedOdorType
        Inherits AnimatTools.Framework.DataObject

#Region " Attributes "

        Protected m_doOdorType As AnimatTools.DataObjects.Physical.OdorType

#End Region

#Region " Properties "

        <Browsable(False)> _
        Public Property OdorType() As AnimatTools.DataObjects.Physical.OdorType
            Get
                Return m_doOdorType
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Physical.OdorType)
                m_doOdorType = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Overrides Property ViewSubProperties() As Boolean
            Get
                Return False
            End Get
            Set(ByVal Value As Boolean)
            End Set
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)
        End Sub

        Public Sub New(ByVal doOdorType As AnimatTools.DataObjects.Physical.OdorType)
            MyBase.New(doOdorType)

            m_doOdorType = doOdorType
        End Sub

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim OrigNode As LinkedOdorType = DirectCast(doOriginal, LinkedOdorType)

            Dim thOrig As LinkedOdorType = DirectCast(OrigNode, LinkedOdorType)
            m_doOdorType = thOrig.m_doOdorType
        End Sub

        Protected Overrides Sub BuildProperties()
        End Sub

#End Region

    End Class

End Namespace
