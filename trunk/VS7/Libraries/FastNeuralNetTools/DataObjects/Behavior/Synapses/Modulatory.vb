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

Namespace DataObjects.Behavior.Synapses

    Public Class Modulated
        Inherits FastSynapse

#Region " Attributes "

        Protected m_strUserText As String = ""
        Protected m_snGain As AnimatTools.Framework.ScaledNumber
        Protected m_lsModulatedSynapse As LinkedSynapse

        'Only used during loading
        Protected m_strModulatedSynapseID As String
        Protected m_strModulatedOriginID As String

#End Region

#Region " Properties "

        Public Overrides ReadOnly Property TypeName() As String
            Get
                Return "Modulatory Synapse"
            End Get
        End Property

        Public Overrides ReadOnly Property NeuralModuleType() As System.Type
            Get
                Return GetType(FastNeuralNetTools.DataObjects.Behavior.NeuralModule)
            End Get
        End Property

        Public Overrides Property ActualDestination() As AnimatTools.DataObjects.Behavior.Node
            Get
                Return MyBase.ActualDestination
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Behavior.Node)
                m_bnDestination = Value

                If Not Value Is Nothing AndAlso TypeOf Value Is AnimatTools.DataObjects.Behavior.Nodes.OffPage Then
                    Dim doNode As AnimatTools.DataObjects.Behavior.Nodes.OffPage = DirectCast(Value, AnimatTools.DataObjects.Behavior.Nodes.OffPage)
                    If Not doNode.LinkedNode Is Nothing AndAlso Not doNode.LinkedNode.Node Is Nothing Then
                        m_lsModulatedSynapse.Node = doNode.LinkedNode.Node
                    Else
                        Throw New System.Exception("The linked node of the destination offpage connector is not set.")
                    End If
                Else
                    m_lsModulatedSynapse.Node = Value
                End If

                UpdateChart()
            End Set
        End Property

        Public Overrides Property Destination() As AnimatTools.DataObjects.Behavior.Node
            Get
                Return MyBase.Destination
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Behavior.Node)
                m_bnDestination = Value

                If Not Value Is Nothing AndAlso TypeOf Value Is AnimatTools.DataObjects.Behavior.Nodes.OffPage Then
                    Dim doNode As AnimatTools.DataObjects.Behavior.Nodes.OffPage = DirectCast(Value, AnimatTools.DataObjects.Behavior.Nodes.OffPage)
                    If Not doNode.LinkedNode Is Nothing AndAlso Not doNode.LinkedNode.Node Is Nothing Then
                        m_lsModulatedSynapse.Node = doNode.LinkedNode.Node
                    Else
                        Throw New System.Exception("The linked node of the destination offpage connector is not set.")
                    End If
                Else
                    m_lsModulatedSynapse.Node = Value
                End If

                UpdateChart()
            End Set
        End Property

        Public Overridable Property UserText() As String
            Get
                Return m_strUserText
            End Get
            Set(ByVal Value As String)
                m_strUserText = Value

                If m_strUserText.Trim.Length > 0 Then
                    Me.Text = m_snGain.Text & vbCrLf & Replace(m_strUserText, vbCrLf, "")
                Else
                    Me.Text = m_snGain.Text
                End If
            End Set
        End Property

        Public Overridable Property Gain() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snGain
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)

                'If they are changing sign of the weight to be negative then lets change the destination arrow.
                If Value.Value < 0 AndAlso m_snGain.Value > 0 Then
                    Me.ArrowDestination.Filled = True
                ElseIf Value.Value > 0 AndAlso m_snGain.Value < 0 Then
                    Me.ArrowDestination.Filled = True
                End If

                m_snGain.CopyData(Value)

                If m_strUserText.Trim.Length > 0 Then
                    Me.Text = m_snGain.Text & vbCrLf & m_strUserText
                Else
                    Me.Text = m_snGain.Text
                End If
            End Set
        End Property

        Public Overridable Property ModulatedSynapse() As LinkedSynapse
            Get
                Return m_lsModulatedSynapse
            End Get
            Set(ByVal Value As LinkedSynapse)
                m_lsModulatedSynapse.Link = Value.Link
            End Set
        End Property

        Public Overrides ReadOnly Property SynapseType() As String
            Get
                Return "Modulated"
            End Get
        End Property

        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return "FastNeuralNetTools.ModulatorySynapse.gif"
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            m_bEnabled = True
            m_lsModulatedSynapse = New LinkedSynapse(m_bnDestination, Nothing)
            m_snGain = New AnimatTools.Framework.ScaledNumber(Me, "Gain", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")

            Me.DrawColor = Color.Black

            Dim myAssembly As System.Reflection.Assembly
            myAssembly = System.Reflection.Assembly.Load("FastNeuralNetTools")

            Me.Image = AnimatTools.Framework.ImageManager.LoadImage(myAssembly, "FastNeuralNetTools.ModulatorySynapse.gif", False)
            Me.Name = "Modulatory Synapse"

            Me.ArrowDestination = New Arrow(Me, AnimatTools.DataObjects.Behavior.Link.enumArrowStyle.Losange, AnimatTools.DataObjects.Behavior.Link.enumArrowSize.Medium, AnimatTools.DataObjects.Behavior.Link.enumArrowAngle.deg30, False)

            Me.Font = New Font("Arial", 12)
            Me.Description = "A modulatory heterosynaptic synapse."
            Me.Text = m_snGain.Text

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim oNewLink As New Synapses.Modulated(doParent)
            oNewLink.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then oNewLink.AfterClone(Me, bCutData, doRoot, oNewLink)
            Return oNewLink
        End Function

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim bnLink As Synapses.Modulated = DirectCast(doOriginal, Synapses.Modulated)

            m_bEnabled = bnLink.m_bEnabled
            m_strUserText = bnLink.m_strUserText
            m_lsModulatedSynapse = DirectCast(bnLink.m_lsModulatedSynapse.Clone(Me, bCutData, doRoot), LinkedSynapse)
            m_snGain = DirectCast(bnLink.m_snGain.Clone(Me, bCutData, doRoot), ScaledNumber)
        End Sub

        Public Overrides Sub SaveNetwork(ByRef oXml As AnimatTools.Interfaces.StdXml, ByRef nmModule As AnimatTools.DataObjects.Behavior.NeuralModule)

            'Only save this as a synapse if the origin node is another FastNeuralNet neuron
            If Not Util.IsTypeOf(Me.Origin.GetType(), GetType(DataObjects.Behavior.Neurons.Normal), False) Then
                Return
            End If

            Dim fnNeuron As DataObjects.Behavior.Neurons.Normal = DirectCast(Me.Origin, DataObjects.Behavior.Neurons.Normal)

            oXml.AddChildElement("CompoundSynapse")
            oXml.IntoElem()

            oXml.AddChildElement("Type", Me.SynapseType)
            oXml.AddChildElement("Enabled", m_bEnabled)
            Util.SaveVector(oXml, "From", New AnimatTools.Framework.Vec3i(Me, fnNeuron.NodeIndex, 0, 0))
            oXml.AddChildElement("Weight", m_snGain.ActualValue)

            oXml.OutOfElem()

        End Sub

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()

            m_Properties.Properties.Remove("Text")
            m_Properties.Properties.Remove("Link Type")

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Modulated Synapse", m_lsModulatedSynapse.GetType(), "ModulatedSynapse", _
                                        "Synapse Properties", "Sets the secondary syanpse that will be modulated.", m_lsModulatedSynapse, _
                                        GetType(AnimatTools.TypeHelpers.DropDownListEditor), GetType(LinkedSynapseTypeConverter)))

            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snGain.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Gain", pbNumberBag.GetType(), "Gain", _
                                        "Synapse Properties", "Sets the gain of this modulatory synaptic connection.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Text", m_strUserText.GetType(), "UserText", _
                                        "Synapse Properties", "Sets or returns the user text associated with the link.", _
                                        m_strUserText, GetType(AnimatTools.TypeHelpers.MultiLineStringTypeEditor)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Synapse Type", GetType(String), "TypeName", _
                                        "Synapse Properties", "Returns the type of this link.", TypeName(), True))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Enabled", GetType(Boolean), "Enabled", _
                                        "Synapse Properties", "Determines if this synapse is enabled or not.", m_bEnabled))

        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()

            If Not m_snGain Is Nothing Then m_snGain.ClearIsDirty()
            If Not m_lsModulatedSynapse Is Nothing Then m_lsModulatedSynapse.ClearIsDirty()
        End Sub

        Public Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)

            Try
                MyBase.LoadData(oXml)

                oXml.IntoElem()

                m_bEnabled = oXml.GetChildBool("Enabled", m_bEnabled)
                m_strUserText = oXml.GetChildString("UserText")
                m_snGain.LoadData(oXml, "Gain")
                m_strModulatedSynapseID = Util.LoadID(oXml, "ModulatedSynapse", True, "")
                m_strModulatedOriginID = Util.LoadID(oXml, "ModulatedOrigin", True, "")

                oXml.OutOfElem()

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overrides Sub InitializeAfterLoad(ByVal iAttempt As Integer)

            Try
                MyBase.InitializeAfterLoad(iAttempt)

                If m_bInitialized Then
                    If m_strModulatedSynapseID.Trim.Length > 0 Then
                        Dim blLink As AnimatTools.DataObjects.Behavior.Link = Me.Organism.FindBehavioralLink(m_strModulatedSynapseID)
                        Dim bnNode As AnimatTools.DataObjects.Behavior.Node = Me.Organism.FindBehavioralNode(m_strModulatedOriginID)

                        m_lsModulatedSynapse = New FastNeuralNetTools.DataObjects.Behavior.LinkedSynapse(bnNode, blLink)
                    Else
                        m_lsModulatedSynapse = New FastNeuralNetTools.DataObjects.Behavior.LinkedSynapse(m_bnDestination, Nothing)
                    End If
                End If

            Catch ex As System.Exception
                m_bInitialized = False
                If iAttempt = 1 Then
                    AnimatTools.Framework.Util.DisplayError(ex)
                End If
            End Try

        End Sub

        Public Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)

            Try
                MyBase.SaveData(oXml)

                oXml.IntoElem() 'Into Node Element

                oXml.AddChildElement("Enabled", m_bEnabled)
                oXml.AddChildElement("UserText", m_strUserText)
                m_snGain.SaveData(oXml, "Gain")

                If Not m_lsModulatedSynapse Is Nothing AndAlso Not m_lsModulatedSynapse.Link Is Nothing Then
                    oXml.AddChildElement("ModulatedSynapseID", m_lsModulatedSynapse.Link.SelectedID)
                    oXml.AddChildElement("ModulatedOriginID", m_lsModulatedSynapse.Node.SelectedID)
                End If

                oXml.OutOfElem() ' Outof Node Element

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

#End Region

#End Region

    End Class

End Namespace