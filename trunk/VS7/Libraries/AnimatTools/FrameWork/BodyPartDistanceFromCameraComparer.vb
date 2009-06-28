Namespace Framework
    Public Class BodyPartDistanceFromCameraComparer
        Implements IComparer


        Public Function Compare(ByVal x As Object, ByVal y As Object) As Integer Implements System.Collections.IComparer.Compare
            Dim bpBody1 As AnimatTools.DataObjects.Physical.BodyPart
            Dim bpBody2 As AnimatTools.DataObjects.Physical.BodyPart

            bpBody1 = DirectCast(x, AnimatTools.DataObjects.Physical.BodyPart)
            bpBody2 = DirectCast(y, AnimatTools.DataObjects.Physical.BodyPart)

            If (bpBody1.DistanceFromCamera > bpBody2.DistanceFromCamera) Then
                Return -1
            ElseIf (bpBody1.DistanceFromCamera < bpBody2.DistanceFromCamera) Then
                Return 1
            Else
                If (bpBody1.BoundingBoxVolume > bpBody2.BoundingBoxVolume) Then
                    Return -1
                ElseIf (bpBody1.BoundingBoxVolume < bpBody2.BoundingBoxVolume) Then
                    Return 1
                End If

                Return 0
            End If
        End Function
    End Class
End Namespace

