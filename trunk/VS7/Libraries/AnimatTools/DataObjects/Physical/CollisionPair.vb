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

Namespace DataObjects.Physical

    Public Class CollisionPair
        Inherits Framework.DataObject

#Region " Attributes "

        Protected m_doStructure As PhysicalStructure

        Protected m_doPart1 As AnimatTools.DataObjects.Physical.RigidBody
        Protected m_doPart2 As AnimatTools.DataObjects.Physical.RigidBody

#End Region

#Region " Properties "

        <Browsable(False)> _
        Public Overridable Property PhysicalStructure() As AnimatTools.DataObjects.Physical.PhysicalStructure
            Get
                Return m_doStructure
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Physical.PhysicalStructure)
                m_doStructure = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property Part1() As AnimatTools.DataObjects.Physical.RigidBody
            Get
                Return m_doPart1
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Physical.RigidBody)
                m_doPart1 = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property Part2() As AnimatTools.DataObjects.Physical.RigidBody
            Get
                Return m_doPart2
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Physical.RigidBody)
                m_doPart2 = Value
            End Set
        End Property

        Public Overridable ReadOnly Property Part1Name() As String
            Get
                If Not m_doPart1 Is Nothing Then
                    Return m_doPart1.Name
                Else
                    Return ""
                End If
            End Get
        End Property

        Public Overridable ReadOnly Property Part2Name() As String
            Get
                If Not m_doPart2 Is Nothing Then
                    Return m_doPart2.Name
                Else
                    Return ""
                End If
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)

            If doParent Is Nothing Then
                Throw New System.Exception("You must pass a physical structure in as the parent of a collision pair.")
            End If

            If Not TypeOf doParent Is AnimatTools.DataObjects.Physical.PhysicalStructure Then
                Throw New System.Exception("You must pass a physical structure in as the parent of a collision pair.")
            End If

            m_doStructure = DirectCast(doParent, AnimatTools.DataObjects.Physical.PhysicalStructure)
            m_doPart1 = Nothing
            m_doPart2 = Nothing
        End Sub

        Public Sub New(ByVal doParent As Framework.DataObject, ByVal doPart1 As AnimatTools.DataObjects.Physical.RigidBody, ByVal doPart2 As AnimatTools.DataObjects.Physical.RigidBody)
            MyBase.New(doParent)

            If doParent Is Nothing Then
                Throw New System.Exception("You must pass a physical structure in as the parent of a collision pair.")
            End If

            If Not TypeOf doParent Is AnimatTools.DataObjects.Physical.PhysicalStructure Then
                Throw New System.Exception("You must pass a physical structure in as the parent of a collision pair.")
            End If

            m_doStructure = DirectCast(doParent, AnimatTools.DataObjects.Physical.PhysicalStructure)
            m_doPart1 = doPart1
            m_doPart2 = doPart2
        End Sub

        Protected Overrides Sub BuildProperties()

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Part1", GetType(String), "Part1Name", _
                                        "Collision Pair Properties", "The first member of the pair of body parts that you want to disable collisions between.", _
                                        Me.Part1Name, True))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Part2", GetType(String), "Part2Name", _
                                        "Collision Pair Properties", "The second member of the pair of body parts that you want to disable collisions between.", _
                                        Me.Part2Name, True))

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim doItem As New CollisionPair(doParent)

            doItem.m_doStructure = m_doStructure
            doItem.m_doPart1 = m_doPart1
            doItem.m_doPart2 = m_doPart2

            Return doItem
        End Function

        Public Overridable Overloads Sub SaveData(ByRef dsSim As Simulation, ByRef oXml As Interfaces.StdXml)

            'Only save this thing if we have good references for BOTH body parts.
            If Not m_doStructure Is Nothing AndAlso Not m_doPart1 Is Nothing AndAlso Not m_doPart2 Is Nothing Then
                If Not m_doStructure.FindBodyPart(m_doPart1.ID, False) Is Nothing AndAlso Not m_doStructure.FindBodyPart(m_doPart2.ID, False) Is Nothing Then

                    oXml.AddChildElement("CollisionPair")
                    oXml.IntoElem()

                    oXml.AddChildElement("Part1ID", m_doPart1.ID)
                    oXml.AddChildElement("Part2ID", m_doPart2.ID)

                    oXml.OutOfElem()
                End If
            End If

        End Sub

        Public Overridable Overloads Sub LoadData(ByRef dsSim As Simulation, ByRef oXml As Interfaces.StdXml)

            oXml.IntoElem() 'Into Camera Element

            m_doPart1 = Nothing
            Dim doPart As DataObjects.Physical.BodyPart
            Dim strID As String = oXml.GetChildString("Part1ID", "")
            If strID.Trim.Length > 0 AndAlso Not m_doStructure Is Nothing Then
                doPart = m_doStructure.FindBodyPart(strID, False)
                If Not doPart Is Nothing Then
                    m_doPart1 = DirectCast(doPart, AnimatTools.DataObjects.Physical.RigidBody)
                End If
            End If

            m_doPart2 = Nothing
            strID = oXml.GetChildString("Part2ID", "")
            If strID.Trim.Length > 0 AndAlso Not m_doStructure Is Nothing Then
                doPart = m_doStructure.FindBodyPart(strID, False)
                If Not doPart Is Nothing Then
                    m_doPart2 = DirectCast(doPart, AnimatTools.DataObjects.Physical.RigidBody)
                End If
            End If

            oXml.OutOfElem() 'Outof Camera Element

        End Sub

        Public Overrides Function ToString() As String
            If Not m_doPart1 Is Nothing AndAlso Not m_doPart2 Is Nothing Then
                Return "(" & m_doPart1.Name & ", " & m_doPart2.Name & ")"
            Else
                Return ""
            End If
        End Function

#End Region

    End Class

End Namespace

