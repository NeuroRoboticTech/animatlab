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

Namespace DataObjects.Behavior

    Public MustInherit Class NeuralModule
        Inherits AnimatTools.Framework.DataObject

#Region " Attributes "

        Protected m_strModuleName As String = ""
        'Protected m_strModuleFilename As String = ""
        Protected m_strModuleType As String = ""
        'Protected m_strNetworkFilename As String = ""
        Protected m_snTimeStep As ScaledNumber

        Protected m_aryNodes As New Collections.SortedNodes(Me)
        Protected m_aryLinks As New Collections.SortedLinks(Me)

        Protected m_doOrganism As DataObjects.Physical.Organism

#End Region

#Region " Properties "

        Public Overrides ReadOnly Property ModuleName() As String
            Get
                If m_strModuleName.Trim.Length = 0 Then
                    Throw New System.Exception("The module name for this neural module was not set to a default value.")
                End If

                Return m_strModuleName
            End Get
        End Property

        Public MustOverride ReadOnly Property ModuleFilename() As String

        Public Overridable ReadOnly Property ModuleType() As String
            Get
                If m_strModuleName.Trim.Length = 0 Then
                    Throw New System.Exception("The module type for this neural module was not set to a default value.")
                End If

                Return m_strModuleType
            End Get
        End Property

        Public MustOverride ReadOnly Property NetworkFilename() As String

        Public Overridable Property TimeStep() As ScaledNumber
            Get
                Return m_snTimeStep
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0.0000001 OrElse Value.ActualValue > 0.05 Then
                    Throw New System.Exception("The time step must be between the range 0.0001 to 50 ms.")
                End If

                m_snTimeStep.CopyData(Value)
            End Set
        End Property

        Public Overridable Property Organism() As DataObjects.Physical.Organism
            Get
                Return m_doOrganism
            End Get
            Set(ByVal Value As DataObjects.Physical.Organism)
                m_doOrganism = Value
            End Set
        End Property

        Public Overridable ReadOnly Property Nodes() As Collections.SortedNodes
            Get
                Return m_aryNodes
            End Get
        End Property

        Public Overridable ReadOnly Property Links() As Collections.SortedLinks
            Get
                Return m_aryLinks
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            m_snTimeStep = New AnimatTools.Framework.ScaledNumber(Me, "TimeStep", 2.5, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
        End Sub

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim OrigModule As AnimatTools.DataObjects.Behavior.NeuralModule = DirectCast(doOriginal, NeuralModule)

            m_strModuleName = OrigModule.m_strModuleName
            m_strModuleType = OrigModule.m_strModuleType
            m_snTimeStep = DirectCast(OrigModule.m_snTimeStep.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_doOrganism = OrigModule.m_doOrganism

        End Sub

#Region " Save Network File "

        Public Overridable Sub PopulateModuleData()
            m_aryNodes.Clear()
            m_aryLinks.Clear()

            'Lets loop through the nodes and links in the organism and add them to this module.
            Dim bnNode As DataObjects.Behavior.Node
            If Not Me.Organism Is Nothing Then
                For Each deEntry As DictionaryEntry In Me.Organism.BehavioralNodes
                    bnNode = DirectCast(deEntry.Value, DataObjects.Behavior.Node)

                    If Me.GetType Is bnNode.NeuralModuleType Then
                        m_aryNodes.Add(bnNode.ID, bnNode)
                    End If
                Next

                Dim blLink As DataObjects.Behavior.Link
                For Each deEntry As DictionaryEntry In Me.Organism.BehavioralLinks
                    blLink = DirectCast(deEntry.Value, DataObjects.Behavior.Link)

                    If Me.GetType Is blLink.NeuralModuleType Then
                        m_aryLinks.Add(blLink.ID, blLink)
                    End If
                Next
            End If

        End Sub


        Public Overridable Sub SaveNetwork(ByRef oXml As Interfaces.StdXml)
            PopulateModuleData()

            If m_aryNodes.Count > 0 Then
                oXml.AddChildElement("NeuralModule")
                oXml.IntoElem()

                oXml.AddChildElement("ModuleName", Me.ModuleName)
                oXml.AddChildElement("ModuleFileName", Me.ModuleFilename)
                oXml.AddChildElement("Type", Me.ModuleType)

                If Util.ExportForStandAloneSim Then
                    SaveNetworkFile(oXml)
                Else
                    oXml.AddChildElement("NeuralNetFile", Me.NetworkFilename)
                End If

                oXml.OutOfElem()

                If Not Util.ExportForStandAloneSim Then
                    SaveNetworkFile(Me.NetworkFilename)
                End If
            Else
                'If we do not have any elements for this neural module and yet a file exists then it must be redundant so delete it.
                Dim strPath As String = Util.GetFilePath(Util.Application.ProjectPath, Me.NetworkFilename)

                If File.Exists(strPath) Then
                    File.Delete(strPath)
                End If
            End If

        End Sub

        Protected Overridable Overloads Sub SaveNetworkFile(ByVal strFilename As String)

            Try
                Dim oXml As New AnimatTools.Interfaces.StdXml

                If strFilename.Trim.Length = 0 Then
                    Throw New System.Exception("The network file name for module '" & Me.ModuleName & "' is blank.")
                End If

                oXml.AddElement("NeuralModule")
                SaveNetworkFile(oXml)
                oXml.Save(Util.GetFilePath(Util.Application.ProjectPath, strFilename))

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected MustOverride Overloads Sub SaveNetworkFile(ByRef oXml As AnimatTools.Interfaces.StdXml)

#End Region

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("ID", Me.ID.GetType(), "ID", _
                                        "Module Properties", "ID", Me.ID, True))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Network Filename", GetType(String), "NetworkFilename", _
                                        "Module Properties", "Sets the filename for this neural network module.", Me.NetworkFilename, True))

            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snTimeStep.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Time Step", pbNumberBag.GetType(), "TimeStep", _
                                        "Module Properties", "Sets integration time step determines the speed and accuracy of the calculation. " & _
                                        "The smaller the value, the more accurate but the slower the calculation.  " & _
                                        "If the value is too large, the calculation may become unstable. " & _
                                        "Acceptable values are in the range 0.0001 to 50 ms.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()

            If Not m_snTimeStep Is Nothing Then m_snTimeStep.ClearIsDirty()
            'm_aryNodes.ClearIsDirty()
            'm_aryLinks.ClearIsDirty()
        End Sub

        Public Overrides Sub LoadData(ByRef oXml As Interfaces.StdXml)

            oXml.IntoElem()  'Into Module Element
            m_snTimeStep.LoadData(oXml, "TimeStep")
            oXml.OutOfElem()  'Outof Module Element

            Me.IsDirty = False
        End Sub

        Public Overrides Sub SaveData(ByRef oXml As Interfaces.StdXml)

            oXml.AddChildElement("Node")
            oXml.IntoElem()  'Into Module Element

            oXml.AddChildElement("AssemblyFile", Me.AssemblyFile)
            oXml.AddChildElement("ClassName", Me.ClassName)

            m_snTimeStep.SaveData(oXml, "TimeStep")

            oXml.OutOfElem()  'Outof Module Element

        End Sub

#End Region

#End Region

    End Class

End Namespace
