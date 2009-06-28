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

    Public Interface IMuscle

        Property StimTension() As DataObjects.Gains.MuscleGains.StimulusTension
        Property LengthTension() As DataObjects.Gains.MuscleGains.LengthTension
        Property VelocityTension() As DataObjects.Gains.MuscleGains.VelocityTension

    End Interface

End Namespace

