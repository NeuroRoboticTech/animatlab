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

    Public Class Normal
        Inherits FastSynapse

#Region " Attributes "

        Protected m_strUserText As String = ""
        Protected m_snWeight As AnimatTools.Framework.ScaledNumber

#End Region

#Region " Properties "

        Public Overrides ReadOnly Property TypeName() As String
            Get
                Return "Normal Synapse"
            End Get
        End Property

        Public Overrides ReadOnly Property NeuralModuleType() As System.Type
            Get
                Return GetType(FastNeuralNetTools.DataObjects.Behavior.NeuralModule)
            End Get
        End Property

        Public Overridable Property UserText() As String
            Get
                Return m_strUserText
            End Get
            Set(ByVal Value As String)
                m_strUserText = Value

                If m_strUserText.Trim.Length > 0 Then
                    Me.Text = m_snWeight.Text & vbCrLf & Replace(m_strUserText, vbCrLf, "")
                Else
                    Me.Text = m_snWeight.Text
                End If
            End Set
        End Property

        Public Overridable Property Weight() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snWeight
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)

                'If they are changing sign of the weight to be negative then lets change the destination arrow.
                If Value.Value < 0 AndAlso m_snWeight.Value > 0 Then
                    Me.ArrowDestination.Style = AnimatTools.DataObjects.Behavior.Link.enumArrowStyle.Circle
                    Me.ArrowDestination.Filled = True
                ElseIf Value.Value > 0 AndAlso m_snWeight.Value < 0 Then
                    Me.ArrowDestination.Style = AnimatTools.DataObjects.Behavior.Link.enumArrowStyle.Fork
                    Me.ArrowDestination.Filled = False
                End If

                m_snWeight.CopyData(Value)

                If m_strUserText.Trim.Length > 0 Then
                    Me.Text = m_snWeight.Text & vbCrLf & m_strUserText
                Else
                    Me.Text = m_snWeight.Text
                End If
            End Set
        End Property

        Public Overrides ReadOnly Property SynapseType() As String
            Get
                Return "Regular"
            End Get
        End Property

        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return "FastNeuralNetTools.ExcitatorySynapse.gif"
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            m_bEnabled = True
            m_snWeight = New AnimatTools.Framework.ScaledNumber(Me, "Weight", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.nano, "Amps", "A")

            Me.DrawColor = Color.Black

            Dim myAssembly As System.Reflection.Assembly
            myAssembly = System.Reflection.Assembly.Load("FastNeuralNetTools")

            Me.Image = AnimatTools.Framework.ImageManager.LoadImage(myAssembly, "FastNeuralNetTools.ExcitatorySynapse.gif", False)
            Me.Name = "Normal Synapse"

            Me.ArrowDestination = New Arrow(Me, AnimatTools.DataObjects.Behavior.Link.enumArrowStyle.Fork, AnimatTools.DataObjects.Behavior.Link.enumArrowSize.Medium, AnimatTools.DataObjects.Behavior.Link.enumArrowAngle.deg30, False)

            Me.Font = New Font("Arial", 12)
            Me.Description = "A normal excitatory or inhibitory synapse."
            Me.Text = m_snWeight.Text

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim oNewLink As New Synapses.Normal(doParent)
            oNewLink.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then oNewLink.AfterClone(Me, bCutData, doRoot, oNewLink)
            Return oNewLink
        End Function

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim bnLink As Synapses.Normal = DirectCast(doOriginal, Synapses.Normal)

            m_bEnabled = bnLink.m_bEnabled
            m_strUserText = bnLink.m_strUserText
            m_snWeight = DirectCast(bnLink.m_snWeight.Clone(Me, bCutData, doRoot), ScaledNumber)
        End Sub

        Public Overrides Sub SaveNetwork(ByRef oXml As AnimatTools.Interfaces.StdXml, ByRef nmModule As AnimatTools.DataObjects.Behavior.NeuralModule)

            'Only save this as a synapse if the origin node is another FastNeuralNet neuron
            If Not Util.IsTypeOf(Me.Origin.GetType(), GetType(DataObjects.Behavior.Neurons.Normal), False) Then
                Return
            End If

            Dim fnNeuron As DataObjects.Behavior.Neurons.Normal = DirectCast(Me.Origin, DataObjects.Behavior.Neurons.Normal)

            oXml.AddChildElement("Synapse")
            oXml.IntoElem()

            oXml.AddChildElement("Enabled", m_bEnabled)
            Util.SaveVector(oXml, "From", New AnimatTools.Framework.Vec3i(Me, fnNeuron.NodeIndex, 0, 0))
            oXml.AddChildElement("Weight", m_snWeight.ActualValue)

            oXml.AddChildElement("CompoundSynapses")
            oXml.IntoElem()

            Dim gsSynapse As DataObjects.Behavior.Synapses.Gated
            Dim msSynapse As DataObjects.Behavior.Synapses.Modulated
            Dim blLink As AnimatTools.DataObjects.Behavior.Link
            Dim bNormal As Boolean = True
            For Each deEntry As DictionaryEntry In m_bnDestination.InLinks

                blLink = DirectCast(deEntry.Value, AnimatTools.DataObjects.Behavior.Link)

                If Not blLink Is Me Then
                    If Util.IsTypeOf(blLink.GetType(), GetType(DataObjects.Behavior.Synapses.Gated), False) Then
                        gsSynapse = DirectCast(blLink, DataObjects.Behavior.Synapses.Gated)
                        If Not gsSynapse.GatedSynapse Is Nothing AndAlso Not gsSynapse.GatedSynapse.Link Is Nothing _
                           AndAlso gsSynapse.GatedSynapse.Link Is Me Then
                            bNormal = False
                            gsSynapse.SaveNetwork(oXml, nmModule)
                        End If
                    End If

                    If Util.IsTypeOf(blLink.GetType(), GetType(DataObjects.Behavior.Synapses.Modulated), False) Then
                        msSynapse = DirectCast(blLink, DataObjects.Behavior.Synapses.Modulated)
                        If Not msSynapse.ModulatedSynapse Is Nothing AndAlso Not msSynapse.ModulatedSynapse.Link Is Nothing _
                           AndAlso msSynapse.ModulatedSynapse.Link Is Me Then
                            bNormal = False
                            msSynapse.SaveNetwork(oXml, nmModule)
                        End If
                    End If
                End If

            Next

            oXml.OutOfElem()

            If bNormal = True Then
                oXml.AddChildElement("Type", Me.SynapseType)
            Else
                oXml.AddChildElement("Type", "Compound")
            End If

            oXml.OutOfElem()

        End Sub

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()

            m_Properties.Properties.Remove("Text")
            m_Properties.Properties.Remove("Link Type")

            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snWeight.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Weight", pbNumberBag.GetType(), "Weight", _
                                        "Synapse Properties", "Sets the weight of this synaptic connection.", pbNumberBag, _
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

            If Not m_snWeight Is Nothing Then m_snWeight.ClearIsDirty()
        End Sub

        Public Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)

            Try
                MyBase.LoadData(oXml)

                oXml.IntoElem()

                m_bEnabled = oXml.GetChildBool("Enabled", m_bEnabled)
                m_strUserText = oXml.GetChildString("UserText")
                m_snWeight.LoadData(oXml, "Weight")

                oXml.OutOfElem()
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)

            Try
                MyBase.SaveData(oXml)

                oXml.IntoElem() 'Into Node Element

                oXml.AddChildElement("Enabled", m_bEnabled)
                oXml.AddChildElement("UserText", m_strUserText)
                m_snWeight.SaveData(oXml, "Weight")

                oXml.OutOfElem() ' Outof Node Element

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

#End Region

#End Region

    End Class

End Namespace
