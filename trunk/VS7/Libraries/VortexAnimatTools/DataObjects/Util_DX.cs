using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections;
using System.IO;
using System.Diagnostics;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using AnimatTools;
using AnimatTools.Framework;
using VortexAnimatTools.DataObjects.Physical.PropertyHelpers;

namespace VortexAnimatTools.DataObjects
{
	/// <summary>
	/// Summary description for Util_DX.
	/// </summary>
	public class Util_DX
	{

		struct IndexEntry
		{
			public Int16 v1, v2, v3;
		}

		#region Face Class

		public class Face
		{
			public int[] aryVertices = new int[3];
			public int[] aryNormals = new int[3];
			public int[] aryTextCoords = new int[3];
		}

		#endregion 

		#region Unit Scale Properties

		public static float BoundingBoxOffset(float fltMaxDim) 
		{ 
			if(Util.Environment.DistanceUnits == AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits.Centimeters)
				return 0.01f;

			AnimatTools.Framework.ScaledNumber snVal = new AnimatTools.Framework.ScaledNumber(null);
			snVal.ActualValue = fltMaxDim;

			switch(snVal.Scale)
			{
				case AnimatTools.Framework.ScaledNumber.enumNumericScale.Tera:
				case AnimatTools.Framework.ScaledNumber.enumNumericScale.Giga:
				case AnimatTools.Framework.ScaledNumber.enumNumericScale.Kilo:
					return 1.0f;
				case AnimatTools.Framework.ScaledNumber.enumNumericScale.None:
					return 0.1f;
				case AnimatTools.Framework.ScaledNumber.enumNumericScale.centi:
					return 0.01f;
				case AnimatTools.Framework.ScaledNumber.enumNumericScale.milli:
					return 0.001f;
				case AnimatTools.Framework.ScaledNumber.enumNumericScale.nano:
				case AnimatTools.Framework.ScaledNumber.enumNumericScale.femto:
				case AnimatTools.Framework.ScaledNumber.enumNumericScale.pico:
					return 0.0001f;
				default:
					return 0.01f;
			}
		}

		public static float CenterSelectionBoxOffset(float fltMaxDim) 
		{ 
			switch(Util.Environment.DistanceUnits)
			{
				case AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits.Kilometers:
				case AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits.Centameters:
					return 1f;
				case AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits.Meters:
					return 0.1f;
				case AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits.Centimeters:
					return 0.01f;
				case AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits.Millimeters:
					return 0.005f;
				default:
					return 0.01f;
			}
		}

		public static float EndSelectionBoxOffset(float fltMaxDim) 
		{ 
			switch(Util.Environment.DistanceUnits)
			{
				case AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits.Kilometers:
				case AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits.Centameters:
					return 0.6f;
				case AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits.Meters:
					return 0.05f;
				case AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits.Centimeters:
					return 0.005f;
				case AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits.Millimeters:
					return 0.0005f;
				default:
					return 0.005f;
			}
		}

		#endregion 

		#region Mesh Loading/Saving

		public static Microsoft.DirectX.Direct3D.Mesh LoadXMesh(Microsoft.DirectX.Direct3D.Device d3dDevice, string strFile, ref Material[] meshMaterials, ref Texture[] meshTextures)
		{
			ExtendedMaterial[] mtrl;

			// Load our mesh
			Mesh d3dMesh = Mesh.FromFile(strFile, MeshFlags.Managed, d3dDevice, out mtrl);

			// If we have any materials, store them
			if ((mtrl != null) && (mtrl.Length > 0))
			{
				meshMaterials = new Material[mtrl.Length];
				meshTextures = new Texture[mtrl.Length];

				// Store each material and texture
				for (int i = 0; i < mtrl.Length; i++)
				{
					meshMaterials[i] = mtrl[i].Material3D;
					if ((mtrl[i].TextureFilename != null) && (mtrl[i].TextureFilename != string.Empty))
					{
						// We have a texture, try to load it
						meshTextures[i] = TextureLoader.FromFile(d3dDevice, @"..\..\" + mtrl[i].TextureFilename);
					}
				}
			}

			return d3dMesh;
		}
		
		protected static void AddVertex(string strLine, ArrayList aryVertices)
		{
			string delimStr = " ";
			char [] delimiter = delimStr.ToCharArray();
			string[] aryParts = strLine.Split(delimiter);

			if(aryParts.Length != 4)
				throw new System.Exception("invalid vertex node found in .obj file.");
			else
			{
				float fltX = float.Parse(aryParts[1]);
				float fltY = float.Parse(aryParts[2]);
				float fltZ = float.Parse(aryParts[3]);

				Microsoft.DirectX.Vector3 vVertice = new Microsoft.DirectX.Vector3(fltX, fltY, fltZ);
				aryVertices.Add(vVertice);
			}
		}
		
		protected static void AddNormal(string strLine, ArrayList aryNormals)
		{
			string delimStr = " ";
			char [] delimiter = delimStr.ToCharArray();
			string[] aryParts = strLine.Split(delimiter);

			if(aryParts.Length != 4)
				throw new System.Exception("invalid normal node found in .obj file.");
			else
			{
				float fltX = float.Parse(aryParts[1]);
				float fltY = float.Parse(aryParts[2]);
				float fltZ = float.Parse(aryParts[3]);

				Microsoft.DirectX.Vector3 vNormal = new Microsoft.DirectX.Vector3(fltX, fltY, fltZ);
				aryNormals.Add(vNormal);
			}
		}
		
		protected static void AddTexCoord(string strLine, ArrayList aryTextures)
		{
			string delimStr = " ";
			char [] delimiter = delimStr.ToCharArray();
			string[] aryParts = strLine.Split(delimiter);

			if(aryParts.Length != 4 && aryParts.Length != 3)
				throw new System.Exception("invalid texture node found in .obj file.");
			else
			{
				float fltX = float.Parse(aryParts[1]);
				float fltY = float.Parse(aryParts[2]);

				Microsoft.DirectX.Vector3 vTexture = new Microsoft.DirectX.Vector3(fltX, fltY, 0);
				aryTextures.Add(vTexture);
			}
		}
		
		protected static void SetFacePart(Face face, string strVertex, int iVertex, int iFaceVertex)
		{
			string delimStr = "/";
			char [] delimiter = delimStr.ToCharArray();
			string[] aryVertexParts;

			aryVertexParts = strVertex.Split(delimiter);

			if(aryVertexParts.Length != 3)
				throw new System.Exception("Invalid face data.");
			else
			{
				int iV=0, iT=0, iN=0;

				//We subtract one from the index value because .obj files are 1 based and not 0 based arrays.
				if(aryVertexParts[0] != null && aryVertexParts[0].Length > 0)
					iV = int.Parse(aryVertexParts[0]) - 1;

				if(aryVertexParts[1] != null && aryVertexParts[1].Length > 0)
					iT = int.Parse(aryVertexParts[1]) - 1;

				if(aryVertexParts[2] != null && aryVertexParts[2].Length > 0)
					iN = int.Parse(aryVertexParts[2]) - 1;
						
				face.aryVertices[iFaceVertex] = iV;
				face.aryTextCoords[iFaceVertex] = iT;
				face.aryNormals[iFaceVertex] = iN;
			}
		}

		protected static void AddFace(string strLine, ArrayList aryFaces)
		{
			string delimStr = " ";
			char [] delimiter = delimStr.ToCharArray();
			string[] aryParts = strLine.Split(delimiter);
			int iVertices = aryParts.Length -1;

			if(aryParts.Length < 4)
				throw new System.Exception("invalid face node found in .obj file.");
			else
			{
				Face face = new Face(), prevFace;
				SetFacePart(face, aryParts[1], 0, 0);
				SetFacePart(face, aryParts[2], 1, 1);
				SetFacePart(face, aryParts[3], 2, 2);
				aryFaces.Add(face);
				prevFace = face;

				for(int iVertex = 3; iVertex<iVertices; iVertex++)
				{
					face = new Face();
					face.aryVertices[0] = prevFace.aryVertices[0];
					face.aryTextCoords[0] = prevFace.aryTextCoords[0];
					face.aryNormals[0] = prevFace.aryNormals[0];
					face.aryVertices[1] = prevFace.aryVertices[2];
					face.aryTextCoords[1] = prevFace.aryTextCoords[2];
					face.aryNormals[1] = prevFace.aryNormals[2];
					SetFacePart(face, aryParts[iVertex+1], iVertex, 2);
					aryFaces.Add(face);
					prevFace = face;
				}

			}
		}

		protected static void LoadObjData(string strFile, ArrayList aryVertices, ArrayList aryNormals, ArrayList aryTextures, ArrayList aryFaces)
		{
			if(File.Exists(strFile))
			{

				using (StreamReader sr = new StreamReader(strFile)) 
				{
					String strLine;

					while ((strLine = sr.ReadLine()) != null) 
					{
						strLine = strLine.Trim();

						if(strLine.Substring(0, 2) == "v ")
							AddVertex(strLine, aryVertices);
						else if(strLine.Substring(0, 3) == "vn ")
							AddNormal(strLine, aryNormals);
						else if(strLine.Substring(0, 3) == "vt ")
							AddTexCoord(strLine, aryTextures);
						else if(strLine.Substring(0, 2) == "f ")
							AddFace(strLine, aryFaces);
					}
				}

				//				Debug.WriteLine("Vertex Count: " + aryVertices.Count.ToString());
				//				Debug.WriteLine("Normal Count: " + aryNormals.Count.ToString());
				//				Debug.WriteLine("Texture Count: " + aryTextures.Count.ToString());
				//				Debug.WriteLine("Face Count: " + aryFaces.Count.ToString());

			}
			else
				throw new System.Exception("File '" + strFile + "' was not found.");
		}

		public static Microsoft.DirectX.Direct3D.Mesh LoadObjMesh(Microsoft.DirectX.Direct3D.Device d3dDevice, string strFile, ref Vector3 vLowerLeftExtent, ref Vector3 vUpperRightExtent)
		{
			ArrayList aryVertices = new ArrayList();
			ArrayList aryNormals = new ArrayList();
			ArrayList aryTextures = new ArrayList();
			ArrayList aryFaces = new ArrayList();
			SortedList aryIndexList = new SortedList();
			bool bExtentFirstSet = false;

			Mesh d3dMesh = null;

			vLowerLeftExtent = new Vector3(0, 0, 0);
			vUpperRightExtent = new Vector3(0, 0, 0);

			LoadObjData(strFile, aryVertices, aryNormals, aryTextures, aryFaces);

			d3dMesh = new Mesh(aryFaces.Count, aryFaces.Count*3, MeshFlags.Managed, CustomVertex.PositionNormalTextured.Format, d3dDevice);

			short[] aryIndices = new short[aryFaces.Count*3];
			using(VertexBuffer vb = d3dMesh.VertexBuffer)
			{
				GraphicsStream data = vb.Lock(0, 0, LockFlags.None);
				short iIndex=0, iItem=0;
				int iV, iN, iT;
				string strKey;

				Microsoft.DirectX.Vector3 vVertex, vNormal, vTexture;
				foreach(Face face in aryFaces)
				{
					for(int iVertex=0; iVertex<3; iVertex++)
					{
						iV = face.aryVertices[iVertex];
						iT = face.aryTextCoords[iVertex];
						iN = face.aryNormals[iVertex];

						strKey = iV + "/" + iT + "/" + iN;  

						if(aryIndexList.Contains(strKey))
						{
							aryIndices[iItem] = (short) aryIndexList[strKey];
							iItem++;
						}
						else
						{
							if(iV < aryVertices.Count)
								vVertex = (Microsoft.DirectX.Vector3) aryVertices[iV];
							else
								vVertex  = new Vector3(0,0,0);
 
							if(iN < aryNormals.Count)
								vNormal = (Microsoft.DirectX.Vector3) aryNormals[iN];
							else
								vNormal  = new Vector3(0,0,0);

							if(iT < aryTextures.Count)
								vTexture = (Microsoft.DirectX.Vector3) aryTextures[iT];
							else
								vTexture  = new Vector3(0,0,0);

							data.Write(new CustomVertex.PositionNormalTextured(vVertex, vNormal, vTexture.X, vTexture.Y));
							aryIndices[iItem] = iIndex;
							aryIndexList.Add(strKey, iIndex);
							iItem++;
							iIndex++;

							if(!bExtentFirstSet || vVertex.X < vLowerLeftExtent.X)
								vLowerLeftExtent.X = vVertex.X;
							if(!bExtentFirstSet || vVertex.X > vUpperRightExtent.X)
								vUpperRightExtent.X = vVertex.X;

							if(!bExtentFirstSet || vVertex.Y < vLowerLeftExtent.Y)
								vLowerLeftExtent.Y = vVertex.Y;
							if(!bExtentFirstSet || vVertex.Y > vUpperRightExtent.Y)
								vUpperRightExtent.Y = vVertex.Y;

							if(!bExtentFirstSet || vVertex.Z < vLowerLeftExtent.Z)
								vLowerLeftExtent.Z = vVertex.Z;
							if(!bExtentFirstSet || vVertex.Z > vUpperRightExtent.Z)
								vUpperRightExtent.Z = vVertex.Z;

							if(!bExtentFirstSet) bExtentFirstSet = true;
						}
					}
				}

				vb.Unlock();
			}

			using (IndexBuffer ib = d3dMesh.IndexBuffer)
			{
				ib.SetData(aryIndices, 0, LockFlags.None);
			}

			return d3dMesh;
		}

		public static void SaveMeshVertices(ref AnimatTools.Interfaces.StdXml oXml, string strName, Mesh myMesh, bool bSaveIndices)
		{
			CustomVertex.PositionNormalTextured[] cvVerts = new CustomVertex.PositionNormalTextured[myMesh.NumberVertices];

			oXml.AddChildElement(strName);
			oXml.IntoElem();  //Into the mesh

			GraphicsStream buffer = myMesh.LockVertexBuffer(LockFlags.ReadOnly);
			AnimatTools.Framework.Vec3d vVertex = new AnimatTools.Framework.Vec3d(null);

			for(int i=0; i<myMesh.NumberVertices; i++)
			{
				buffer.Position = i * myMesh.NumberBytesPerVertex;
				cvVerts[i] = (CustomVertex.PositionNormalTextured)buffer.Read(typeof(CustomVertex.PositionNormalTextured));
				//System.Diagnostics.Debug.WriteLine("v " + cvVerts[i].X + " " + cvVerts[i].Y + " " + cvVerts[i].Z);

				vVertex.X = cvVerts[i].X;// - this.AbsoluteLocation.X;
				vVertex.Y = cvVerts[i].Y;// - this.AbsoluteLocation.Y;
				vVertex.Z = cvVerts[i].Z;// - this.AbsoluteLocation.Z;
				
				Util.SaveVector(ref oXml, "Vector", vVertex);

				//Debug.WriteLine("data.Write(new CustomVertex.PositionTextured(" + vVertex.X + "f, " + vVertex.Y + "f, " + vVertex.Z + "f, 0f, 0f));");
				//if(i%3 == 0) Debug.WriteLine("");
				//Debug.WriteLine("V: " + i + "  (" + cvVerts[i].X + ", " + cvVerts[i].Y + ", " + cvVerts[i].Z + ", " + cvVerts[i].Nx + ", " + cvVerts[i].Ny + ", " + cvVerts[i].Nz + ")");
			}
			myMesh.UnlockVertexBuffer();

			if(bSaveIndices)
			{
				using (IndexBuffer ib = myMesh.IndexBuffer)
				{
					GraphicsStream gs = ib.Lock(0, myMesh.NumberFaces*3*2, LockFlags.ReadOnly);
					
					string strIndex = "";
					short iIndex;
					for(int i=0; i<myMesh.NumberFaces*3; i++)
					{	
						gs.Position = i * 2;
						iIndex = (short) gs.Read(typeof(short));

						strIndex += iIndex.ToString();
						if(i<gs.Length-1) strIndex += ",";
					}

					oXml.AddChildElement("IndexBuffer", strIndex);

					ib.Unlock();
				}
			}

			oXml.OutOfElem();  //Out of the mesh
		}

		//		public static void SaveMeshVertices(ref AnimatTools.Interfaces.StdXml oXml, string strName, Mesh myMesh, bool bSaveIndices)
		//		{
		//			CustomVertex.PositionOnly[] cvVerts = new CustomVertex.PositionOnly[myMesh.NumberVertices];
		//
		//			oXml.AddChildElement(strName);
		//			oXml.IntoElem();  //Into the mesh
		//
		//			GraphicsStream buffer = myMesh.LockVertexBuffer(LockFlags.ReadOnly);
		//			AnimatTools.Framework.Vec3d vVertex = new AnimatTools.Framework.Vec3d(null);
		//
		//			for(int i=0; i<myMesh.NumberVertices; i++)
		//			{
		//				buffer.Position = i * myMesh.NumberBytesPerVertex;
		//				cvVerts[i] = (CustomVertex.PositionOnly)buffer.Read(typeof(CustomVertex.PositionOnly));
		//				//System.Diagnostics.Debug.WriteLine("v " + cvVerts[i].X + " " + cvVerts[i].Y + " " + cvVerts[i].Z);
		//
		//				vVertex.X = cvVerts[i].X;// - this.AbsoluteLocation.X;
		//				vVertex.Y = cvVerts[i].Y;// - this.AbsoluteLocation.Y;
		//				vVertex.Z = cvVerts[i].Z;// - this.AbsoluteLocation.Z;
		//				
		//				Util.SaveVector(ref oXml, "Vector", vVertex);
		//
		//				//Debug.WriteLine("data.Write(new CustomVertex.PositionTextured(" + vVertex.X + "f, " + vVertex.Y + "f, " + vVertex.Z + "f, 0f, 0f));");
		//			}
		//			myMesh.UnlockVertexBuffer();
		//
		//			if(bSaveIndices)
		//			{
		//				using (IndexBuffer ib = myMesh.IndexBuffer)
		//				{
		//					GraphicsStream gs = ib.Lock(0, myMesh.NumberFaces*3*2, LockFlags.ReadOnly);
		//					
		//					string strIndex = "";
		//					short iIndex;
		//					for(int i=0; i<myMesh.NumberFaces*3; i++)
		//					{	
		//						gs.Position = i * 2;
		//						iIndex = (short) gs.Read(typeof(short));
		//
		//						strIndex += iIndex.ToString();
		//						if(i<gs.Length-1) strIndex += ",";
		//					}
		//
		//					oXml.AddChildElement("IndexBuffer", strIndex);
		//
		//					ib.Unlock();
		//				}
		//			}
		//
		//			oXml.OutOfElem();  //Out of the mesh
		//		}

		#endregion 

		#region Mesh Manipulation

		public static void ScaleMesh(Mesh d3dMesh, Vec3d vSize)
		{ScaleMesh(d3dMesh, (float) vSize.X, (float) vSize.Y, (float) vSize.Z);}

		public static void ScaleMesh(Mesh d3dMesh, float fltX, float fltY, float fltZ)
		{
			if(fltX <= 0 || fltY <= 0 || fltZ <= 0)
				throw new System.Exception("Invalid scale value specified.");

			if(fltX == 1 && fltY == 1 && fltZ == 1)
				return;

			CustomVertex.PositionNormalTextured vVertex;
			GraphicsStream buffer = d3dMesh.LockVertexBuffer(LockFlags.None);

			for(int i=0; i<d3dMesh.NumberVertices; i++)
			{
				buffer.Position = i * d3dMesh.NumberBytesPerVertex;
				vVertex = (CustomVertex.PositionNormalTextured)buffer.Read(typeof(CustomVertex.PositionNormalTextured));

				vVertex.X *= (float) fltX;
				vVertex.Y *= (float) fltY;
				vVertex.Z *= (float) fltZ; 

				buffer.Position = i * d3dMesh.NumberBytesPerVertex;
				buffer.Write(new CustomVertex.PositionNormalTextured(vVertex.X, vVertex.Y, vVertex.Z, vVertex.Nx, vVertex.Ny, vVertex.Nz, vVertex.Tu, vVertex.Tv));
			}
			d3dMesh.UnlockVertexBuffer();
		}

		public static Vec3d FindScaleFactor(Vec3d vFirst, Vec3d vSecond)
		{
			Vec3d vSizeDiff = new Vec3d(null, 0, 0, 0);

			vSizeDiff.X = vFirst.X/vSecond.X;
			vSizeDiff.Y = vFirst.Y/vSecond.Y;
			vSizeDiff.Z = vFirst.Z/vSecond.Z;

			return vSizeDiff;
		}

		public static void ColorReceptiveField(Mesh d3dMesh, AnimatTools.Framework.Vec3d vFieldPoint, AnimatTools.DataObjects.Gain gnGain)
		{
			CustomVertex.PositionNormalColored vVertex;
			GraphicsStream buffer = d3dMesh.LockVertexBuffer(LockFlags.None);

			float fltBaseVal = (float) gnGain.CalculateGain(0);
			float fltDist, fltGain=0;

			for(int i=0; i<d3dMesh.NumberVertices; i++)
			{
				buffer.Position = i * d3dMesh.NumberBytesPerVertex;
				vVertex = (CustomVertex.PositionNormalColored)buffer.Read(typeof(CustomVertex.PositionNormalColored));
				
				//dwc changes
				fltDist = (float) Distance(vFieldPoint, vVertex.X, vVertex.Y, vVertex.Z) * Util.Environment.DistanceUnitValue;
				fltGain = (float) gnGain.CalculateGain(fltDist);
				fltGain = 255 - (255*(fltGain/fltBaseVal));
				if(fltGain < 0) fltGain = 0;
				if(fltGain>255) fltGain = 255;

				vVertex.Color = Color.FromArgb(255, (int) fltGain, (int) fltGain).ToArgb();

				buffer.Position = i * d3dMesh.NumberBytesPerVertex;
				buffer.Write(new CustomVertex.PositionNormalColored(vVertex.X, vVertex.Y, vVertex.Z, vVertex.Nx, vVertex.Ny, vVertex.Nz, vVertex.Color));
			}
			d3dMesh.UnlockVertexBuffer();
		}

		//Converts the positiononly vertices generated from the Mesh.?? creation functions to be positionnomralcolored instead.
		public static void ConvertPosOnlyVertexToPosNormCol(Microsoft.DirectX.Direct3D.Device d3dDevice, ref Mesh meshOriginal)
		{
			Mesh pTempMesh = meshOriginal.Clone(meshOriginal.Options.Value, meshOriginal.VertexFormat | VertexFormats.Normal | VertexFormats.Diffuse, d3dDevice);
			pTempMesh.ComputeNormals();
			meshOriginal.Dispose();
			meshOriginal = pTempMesh;
		}

		#endregion 

		#region Mesh Creation

		public static Mesh CreateBox(Microsoft.DirectX.Direct3D.Device d3dDevice, Vec3d vSize)
		{return CreateBox(d3dDevice, (float) vSize.X, (float) vSize.Y, (float) vSize.Z);}

		public static Mesh CreateBox(Microsoft.DirectX.Direct3D.Device d3dDevice, float fltX, float fltY, float fltZ)
		{
			//VertexBuffer vb = (VertexBuffer)sender;
			ArrayList aryStrips = new ArrayList();
			ArrayList aryTriangleList;
			CustomVertex.PositionNormalTextured[] verts;
			Microsoft.DirectX.Vector3 vNormal = new Vector3();

			verts = new CustomVertex.PositionNormalTextured[4];
			vNormal.X = 0; vNormal.Y = 1; vNormal.Z = 0;
			verts[0] = new CustomVertex.PositionNormalTextured(-fltX*0.5f,  fltY*0.5f, -fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 0, 0);
			verts[1] = new CustomVertex.PositionNormalTextured(-fltX*0.5f,  fltY*0.5f,  fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 0, 1);
			verts[2] = new CustomVertex.PositionNormalTextured( fltX*0.5f,  fltY*0.5f, -fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 1, 0);
			verts[3] = new CustomVertex.PositionNormalTextured( fltX*0.5f,  fltY*0.5f,  fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 1, 1);
			aryStrips.Add(verts);

			verts = new CustomVertex.PositionNormalTextured[4];
			vNormal.X = 0; vNormal.Y = -1; vNormal.Z = 0;
			verts[0] = new CustomVertex.PositionNormalTextured(-fltX*0.5f, -fltY*0.5f, -fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 0, 0);
			verts[1] = new CustomVertex.PositionNormalTextured( fltX*0.5f, -fltY*0.5f, -fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 0, 1);
			verts[2] = new CustomVertex.PositionNormalTextured(-fltX*0.5f, -fltY*0.5f,  fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 1, 0);
			verts[3] = new CustomVertex.PositionNormalTextured( fltX*0.5f, -fltY*0.5f,  fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 1, 1);
			aryStrips.Add(verts);

			verts = new CustomVertex.PositionNormalTextured[4];
			vNormal.X = 1; vNormal.Y = 0; vNormal.Z = 0;
			verts[0] = new CustomVertex.PositionNormalTextured( fltX*0.5f, -fltY*0.5f, -fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 0, 0);
			verts[1] = new CustomVertex.PositionNormalTextured( fltX*0.5f,  fltY*0.5f, -fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 0, 1);
			verts[2] = new CustomVertex.PositionNormalTextured( fltX*0.5f, -fltY*0.5f,  fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 1, 0);
			verts[3] = new CustomVertex.PositionNormalTextured( fltX*0.5f,  fltY*0.5f,  fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 1, 1);
			aryStrips.Add(verts);

			verts = new CustomVertex.PositionNormalTextured[4];
			vNormal.X = -1; vNormal.Y = 0; vNormal.Z = 0;
			verts[0] = new CustomVertex.PositionNormalTextured(-fltX*0.5f, -fltY*0.5f, -fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 0, 0);
			verts[1] = new CustomVertex.PositionNormalTextured(-fltX*0.5f, -fltY*0.5f,  fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 0, 1);
			verts[2] = new CustomVertex.PositionNormalTextured(-fltX*0.5f,  fltY*0.5f, -fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 1, 0);
			verts[3] = new CustomVertex.PositionNormalTextured(-fltX*0.5f,  fltY*0.5f,  fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 1, 1);
			aryStrips.Add(verts);

			verts = new CustomVertex.PositionNormalTextured[4];
			vNormal.X = 0; vNormal.Y = 0; vNormal.Z = 1;
			verts[0] = new CustomVertex.PositionNormalTextured(-fltX*0.5f, -fltY*0.5f,  fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 0, 0);
			verts[1] = new CustomVertex.PositionNormalTextured( fltX*0.5f, -fltY*0.5f,  fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 0, 1);
			verts[2] = new CustomVertex.PositionNormalTextured(-fltX*0.5f,  fltY*0.5f,  fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 1, 0);
			verts[3] = new CustomVertex.PositionNormalTextured( fltX*0.5f,  fltY*0.5f,  fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 1, 1);
			aryStrips.Add(verts);

			verts = new CustomVertex.PositionNormalTextured[4];
			vNormal.X = 0; vNormal.Y = 0; vNormal.Z = -1;
			verts[0] = new CustomVertex.PositionNormalTextured(-fltX*0.5f, -fltY*0.5f, -fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 0, 0);
			verts[1] = new CustomVertex.PositionNormalTextured(-fltX*0.5f,  fltY*0.5f, -fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 0, 1);
			verts[2] = new CustomVertex.PositionNormalTextured( fltX*0.5f, -fltY*0.5f, -fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 1, 0);
			verts[3] = new CustomVertex.PositionNormalTextured( fltX*0.5f,  fltY*0.5f, -fltZ*0.5f, vNormal.X, vNormal.Y, vNormal.Z, 1, 1);
			aryStrips.Add(verts);

			aryTriangleList = ConvertTriStripToTriList(aryStrips, true);
			return CreateMeshFromTriangleList(d3dDevice, aryTriangleList);
		}

		public static Mesh CreateBox(Microsoft.DirectX.Direct3D.Device d3dDevice, float fltX, float fltY, float fltZ, int iXDivs, int iYDivs, int iZDivs)
		{
			//VertexBuffer vb = (VertexBuffer)sender;
			ArrayList aryStrips = new ArrayList();
			ArrayList aryTriangleList;
			CustomVertex.PositionNormalColored[] verts;
			Microsoft.DirectX.Vector3 vNormal = new Vector3();

			float fltXDiv = fltX/iXDivs;
			float fltYDiv = fltY/iYDivs;
			float fltZDiv = fltZ/iZDivs;

			float x1, x2, y1, y2, z1, z2;

			//Go through and create the top and bottom panes.
			y1 = -fltY*0.5f; y2 = fltY*0.5f;
			for(int iXDiv=0; iXDiv<iXDivs; iXDiv++)
			{
				x1 = -fltX*0.5f + (fltXDiv*iXDiv);
				x2 = -fltX*0.5f + (fltXDiv*(iXDiv+1));

				for(int iZDiv=0; iZDiv<iZDivs; iZDiv++)
				{
					z1 = -fltZ*0.5f + (fltZDiv*iZDiv);
					z2 = -fltZ*0.5f + (fltZDiv*(iZDiv+1));

					verts = new CustomVertex.PositionNormalColored[4];
					verts[0] = new CustomVertex.PositionNormalColored(x1, y1, z1, 0, -1, 0, Color.White.ToArgb());
					verts[1] = new CustomVertex.PositionNormalColored(x2, y1, z1, 0, -1, 0, Color.White.ToArgb());
					verts[2] = new CustomVertex.PositionNormalColored(x1, y1, z2, 0, -1, 0, Color.White.ToArgb());
					verts[3] = new CustomVertex.PositionNormalColored(x2, y1, z2, 0, -1, 0, Color.White.ToArgb());
					aryStrips.Add(verts);

					verts = new CustomVertex.PositionNormalColored[4];
					verts[0] = new CustomVertex.PositionNormalColored(x1, y2, z1, 0, 1, 0, Color.Blue.ToArgb());
					verts[1] = new CustomVertex.PositionNormalColored(x1, y2, z2, 0, 1, 0, Color.Blue.ToArgb());
					verts[2] = new CustomVertex.PositionNormalColored(x2, y2, z1, 0, 1, 0, Color.Blue.ToArgb());
					verts[3] = new CustomVertex.PositionNormalColored(x2, y2, z2, 0, 1, 0, Color.Blue.ToArgb());
					aryStrips.Add(verts);
				}
			}

			//Go through and create the left and right panes.
			x1 = -fltX*0.5f; x2 = fltX*0.5f;
			for(int iYDiv=0; iYDiv<iYDivs; iYDiv++)
			{
				y1 = -fltY*0.5f + (fltYDiv*iYDiv);
				y2 = -fltY*0.5f + (fltYDiv*(iYDiv+1));

				for(int iZDiv=0; iZDiv<iZDivs; iZDiv++)
				{
					z1 = -fltZ*0.5f + (fltZDiv*iZDiv);
					z2 = -fltZ*0.5f + (fltZDiv*(iZDiv+1));

					verts = new CustomVertex.PositionNormalColored[4];
					verts[0] = new CustomVertex.PositionNormalColored(x1, y1, z1, -1, 0, 0, Color.Red.ToArgb());
					verts[1] = new CustomVertex.PositionNormalColored(x1, y1, z2, -1, 0, 0, Color.Red.ToArgb());
					verts[2] = new CustomVertex.PositionNormalColored(x1, y2, z1, -1, 0, 0, Color.Red.ToArgb());
					verts[3] = new CustomVertex.PositionNormalColored(x1, y2, z2, -1, 0, 0, Color.Red.ToArgb());
					aryStrips.Add(verts);

					verts = new CustomVertex.PositionNormalColored[4];
					verts[0] = new CustomVertex.PositionNormalColored(x2, y1, z1, 1, 0, 0, Color.Green.ToArgb());
					verts[1] = new CustomVertex.PositionNormalColored(x2, y2, z1, 1, 0, 0, Color.Green.ToArgb());
					verts[2] = new CustomVertex.PositionNormalColored(x2, y1, z2, 1, 0, 0, Color.Green.ToArgb());
					verts[3] = new CustomVertex.PositionNormalColored(x2, y2, z2, 1, 0, 0, Color.Green.ToArgb());
					aryStrips.Add(verts);
				}
			}

			//Go through and create the front and back panes.
			z1 = -fltZ*0.5f; z2 = fltZ*0.5f;
			for(int iYDiv=0; iYDiv<iYDivs; iYDiv++)
			{
				y1 = -fltY*0.5f + (fltYDiv*iYDiv);
				y2 = -fltY*0.5f + (fltYDiv*(iYDiv+1));

				for(int iXDiv=0; iXDiv<iXDivs; iXDiv++)
				{
					x1 = -fltX*0.5f + (fltXDiv*iXDiv);
					x2 = -fltX*0.5f + (fltXDiv*(iXDiv+1));

					verts = new CustomVertex.PositionNormalColored[4];
					verts[0] = new CustomVertex.PositionNormalColored(x1, y1, z1, 0, 0, -1, Color.Yellow.ToArgb());
					verts[1] = new CustomVertex.PositionNormalColored(x1, y2, z1, 0, 0, -1, Color.Yellow.ToArgb());
					verts[2] = new CustomVertex.PositionNormalColored(x2, y1, z1, 0, 0, -1, Color.Yellow.ToArgb());
					verts[3] = new CustomVertex.PositionNormalColored(x2, y2, z1, 0, 0, -1, Color.Yellow.ToArgb());
					aryStrips.Add(verts);

					verts = new CustomVertex.PositionNormalColored[4];
					verts[0] = new CustomVertex.PositionNormalColored(x1, y1, z2, 0, 0, 1, Color.Violet.ToArgb());
					verts[1] = new CustomVertex.PositionNormalColored(x2, y1, z2, 0, 0, 1, Color.Violet.ToArgb());
					verts[2] = new CustomVertex.PositionNormalColored(x1, y2, z2, 0, 0, 1, Color.Violet.ToArgb());
					verts[3] = new CustomVertex.PositionNormalColored(x2, y2, z2, 0, 0, 1, Color.Violet.ToArgb());
					aryStrips.Add(verts);
				}
			}

			aryTriangleList = ConvertTriStripToTriList(aryStrips, true, new CustomVertex.PositionNormalColored(0,0,0,0,0,0,0));
			return CreateMeshFromTriangleList(d3dDevice, aryTriangleList, new CustomVertex.PositionNormalColored(0,0,0,0,0,0,0));
		}

		public static Mesh CreateSphere(Microsoft.DirectX.Direct3D.Device d3dDevice, float fltRadius, int iRings, int iSides)
		{
			CustomVertex.PositionNormalTextured[] verts;
			ArrayList aryStrips = new ArrayList();
			ArrayList aryTriangleList;

			//initialize the strips
			for(int iStrip=0; iStrip<iRings; iStrip++)
				aryStrips.Add(new CustomVertex.PositionNormalTextured[2*(iSides+1)]);

			//Initialize the rings.
			Vector3[] aryVertices = new Vector3[iRings+1];
			Vector3[] aryNormals = new Vector3[iRings+1];
			float[] aryCoords = new float[iRings+1];
			for(int iRing=0; iRing<iRings+1; iRing++)
			{
				float fltAngle = ((float)iRing/iRings) * (float) Math.PI;

				aryNormals[iRing] = new Vector3(0, (float) Math.Sin(fltAngle), (float) Math.Cos(fltAngle));
				aryVertices[iRing] = new Vector3(aryNormals[iRing].X*fltRadius, aryNormals[iRing].Y*fltRadius, aryNormals[iRing].Z*fltRadius);

				aryCoords[iRing] = ((float)iRing/iRings);
			}

			/* Then transform that arc to each angle. */
			Vector3 vVertex, vNormal;
			float fltU, fltV;
			for(int iSide=0; iSide<iSides+1; iSide++)
			{
				float fltAroundAngle = ((float)iSide/iSides) * 2 * (float) Math.PI;
				Microsoft.DirectX.Matrix mR = Matrix3MakeRotationZ(fltAroundAngle);

				for(int iRing=0; iRing<iRings; iRing++)
				{
					verts = (CustomVertex.PositionNormalTextured[]) aryStrips[iRing];
					
					vVertex = Matrix3MultiplyVector(mR, aryVertices[iRing]);
					vNormal = Matrix3MultiplyVector(mR, aryNormals[iRing]);
					fltU = ((float)iSide/iSides);
					fltV = aryCoords[iRing];
					
					verts[iSide*2] = new CustomVertex.PositionNormalTextured(vVertex.X, vVertex.Y, vVertex.Z, vNormal.X, vNormal.Y, vNormal.Z, fltU, fltV);

					vVertex = Matrix3MultiplyVector(mR, aryVertices[iRing+1]);
					vNormal = Matrix3MultiplyVector(mR, aryNormals[iRing+1]);
					fltU = ((float)iSide/iSides);
					fltV = aryCoords[iRing+1];
					
					verts[(iSide*2)+1] = new CustomVertex.PositionNormalTextured(vVertex.X, vVertex.Y, vVertex.Z, vNormal.X, vNormal.Y, vNormal.Z, fltU, fltV);
				}
			}

			aryTriangleList = ConvertTriStripToTriList(aryStrips, false);
			return CreateMeshFromTriangleList(d3dDevice, aryTriangleList);
		}

		public static Mesh CreateCylinder(Microsoft.DirectX.Direct3D.Device d3dDevice, float fltBottomRadius, float fltTopRadius, float fltHeight, int iSides)
		{return CreateCylinder(d3dDevice, fltBottomRadius, fltTopRadius, fltHeight*0.5f, -fltHeight*0.5f, iSides);}

		public static Mesh CreateCylinder(Microsoft.DirectX.Direct3D.Device d3dDevice, float fltBottomRadius, float fltTopRadius, float fltBottom, float fltTop, int iSides)
		{
			int j;
			ArrayList aryStrips = new ArrayList();
			ArrayList aryLastStrip = new ArrayList();
			ArrayList aryTriangleList1;
			ArrayList aryTriangleList2;
			ArrayList aryCombinedList;

			Vector3[] vVertices = new Vector3[2];
			Vector3 vNormal = new Vector3();

			CustomVertex.PositionNormalTextured[] aryTopPoly = new CustomVertex.PositionNormalTextured[iSides+1];
			CustomVertex.PositionNormalTextured[] aryBottomPoly = new CustomVertex.PositionNormalTextured[iSides+1];

			//initialize the strips
			//for(int iStrip=0; iStrip<3; iStrip++)
			//	aryStrips.Add(new CustomVertex.PositionNormalTextured[2*(iSides+1)]);

			aryStrips.Add(new CustomVertex.PositionNormalTextured[iSides]);
			aryStrips.Add(new CustomVertex.PositionNormalTextured[2*(iSides+1)]);
			aryLastStrip.Add(new CustomVertex.PositionNormalTextured[iSides]);

			CustomVertex.PositionNormalTextured[] verts0 = (CustomVertex.PositionNormalTextured[]) aryStrips[0];
			CustomVertex.PositionNormalTextured[] verts1 = (CustomVertex.PositionNormalTextured[]) aryStrips[1];
			CustomVertex.PositionNormalTextured[] verts2 = (CustomVertex.PositionNormalTextured[]) aryLastStrip[0];

			vVertices[0] = new Vector3(fltTopRadius, 0, fltTop);
			vVertices[1] = new Vector3(fltBottomRadius, 0, fltBottom);
			vNormal = new Vector3(fltBottom-fltTop, 0, fltTopRadius-fltBottomRadius);
			vNormal.Normalize();

			// Work around cylinder to create side and end polygons. 
			Vector3 vVertTmp1, vVertTmp2, vNormTmp;
			float fltU;
			for(j=1; j<iSides+1; j++)
			{
				float fltAroundAngle = ((float)j/iSides) * 2 * (float) Math.PI;
				Matrix mR = Matrix3MakeRotationZ(fltAroundAngle);

				vVertTmp1 = Matrix3MultiplyVector(mR, vVertices[0]);

				aryTopPoly[j] = new CustomVertex.PositionNormalTextured(vVertTmp1.X, vVertTmp1.Y, vVertTmp1.Z, 0, 0, -1, (float) ((Math.Cos(fltAroundAngle)+1)*(float)0.5), (float) ((Math.Sin(fltAroundAngle)+1)*(float)0.5));

				vVertTmp2 = Matrix3MultiplyVector(mR, vVertices[1]);

				aryBottomPoly[j] = new CustomVertex.PositionNormalTextured(vVertTmp2.X, vVertTmp2.Y, vVertTmp2.Z, 0, 0, 1, (float) ((Math.Cos(fltAroundAngle)+1)*(float)0.5), (float) ((Math.Sin(fltAroundAngle)+1)*(float)0.5));

				vNormTmp = Matrix3MultiplyVector(mR, vNormal);
				fltU = ((float)j/iSides);
				verts1[j*2] = new CustomVertex.PositionNormalTextured(vVertTmp1.X, vVertTmp1.Y, vVertTmp1.Z, vNormTmp.X, vNormTmp.Y, vNormTmp.Z, fltU, 0);
				verts1[(j*2)+1] = new CustomVertex.PositionNormalTextured(vVertTmp2.X, vVertTmp2.Y, vVertTmp2.Z, vNormTmp.X, vNormTmp.Y, vNormTmp.Z, fltU, 1);
			}

			// CONVERT POLYGONS INTO STRIPS
			int i = 0; // first vertex 
			j = iSides-1; // last vertex 
			int k = 0; // position in strip 

			while(i <= j)
			{
				verts0[k] = aryBottomPoly[i];
				verts2[k] = aryTopPoly[i];
				k++;

				if(i != j)
				{
					verts0[k] = aryBottomPoly[j];
					verts2[k] = aryTopPoly[j];
					k++;
				}

				i++; j--;
			}

			aryTriangleList1 = ConvertTriStripToTriList(aryStrips, true);
			aryTriangleList2 = ConvertTriStripToTriList(aryLastStrip, false);
			aryCombinedList = CombineTriangleLists(aryTriangleList1, aryTriangleList2);
			return CreateMeshFromTriangleList(d3dDevice, aryCombinedList);
		}

		public static Mesh CreateConeLimit(Microsoft.DirectX.Direct3D.Device d3dDevice, float fltHalfAngle, float fltHeight, int iSides)
		{
			Mesh mesh;
			AttributeRange ar = new AttributeRange();			

			float fltRotAmnt = Geometry.DegreeToRadian(360.0f/iSides);

			//create indices
			short[] aryIndices = new short[iSides * 3];

			//create vertices
			CustomVertex.PositionNormal[] aryVerts = new CustomVertex.PositionNormal[iSides + 1];

			//create mesh with desired vertex format and desired size
			mesh = new Mesh(aryIndices.Length/3, aryVerts.Length, MeshFlags.SystemMemory, CustomVertex.PositionNormal.Format, d3dDevice);

			//caclulate the bottom radius vertices
			Vector3 v3Current = new Vector3(0, 0,0);			
			aryVerts[0].Position = v3Current;

			v3Current.Z = fltHeight;
			v3Current.TransformCoordinate(Matrix.RotationX(fltHalfAngle));
			
			aryVerts[1].Position = v3Current;


			for(int i=2; i<iSides + 1; i++)
			{
				v3Current.TransformCoordinate(Matrix.RotationZ(fltRotAmnt));
				aryVerts[i].Position = v3Current;							
			}					

			//calculate the indices
			int j =0;
			for(int i=0; i<aryIndices.Length; i+=3)
			{
				//get first triangle
				aryIndices[i] = (short)(0);
				aryIndices[i+1] = (short)(j + 2);
				aryIndices[i+2] = (short)(j + 1);				
				
				if(i == aryIndices.Length - 3)
				{
					aryIndices[i] =	(short)(0);
					aryIndices[i+1] = (short)(1);
					aryIndices[i+2] = (short)(j + 1);
					

					/*	TODO:  Remove when done
					Debug.WriteLine("\nj = " + j.ToString());
					Debug.WriteLine(aryIndices[i]);
					Debug.WriteLine(aryIndices[i+1]);
					Debug.WriteLine(aryIndices[i+2]);
					Debug.WriteLine(aryIndices[i+3]);
					Debug.WriteLine(aryIndices[i+4]);
					Debug.WriteLine(aryIndices[i+5]);
					*/
				}
				j++;
			}			

//			aryIndices[0] = 0;
//			aryIndices[1] = 2;
//			aryIndices[2] = 1;
//			aryIndices[3] = 0;
//			aryIndices[4] = 3;
//			aryIndices[5] = 2;
//			aryIndices[6] = 0;
//			aryIndices[7] = 4;
//			aryIndices[8] = 3;
//			aryIndices[9] = 0;
//			aryIndices[10] = 1;
//			aryIndices[11] = 4;			

			ar.AttributeId = 0;
			ar.FaceStart = 0;
			ar.FaceCount = aryIndices.Length/3;
			ar.VertexStart = 0;
			ar.VertexCount = aryVerts.Length;

			//set the mesh
			mesh.VertexBuffer.SetData(aryVerts, 0, LockFlags.None);
			mesh.IndexBuffer.SetData(aryIndices, 0, LockFlags.None);
			mesh.SetAttributeTable(new AttributeRange[]{ar});		

			mesh.ComputeNormals();

			return (mesh);
		}

		protected static ArrayList CombineTriangleLists(ArrayList aryList1, ArrayList aryList2)
		{
			ArrayList aryCombined = new ArrayList();

			foreach(CustomVertex.PositionNormalTextured vVertex in aryList1)
				aryCombined.Add(vVertex);

			foreach(CustomVertex.PositionNormalTextured vVertex in aryList2)
				aryCombined.Add(vVertex);
			
			return aryCombined;
		}

		public static Mesh CreateBillboard(Microsoft.DirectX.Direct3D.Device d3dDevice, float fltSizeX, float fltSizeY)
		{
			ArrayList aryTriangleList = new ArrayList();
			fltSizeX = fltSizeX/2.0f;
			fltSizeY = fltSizeY/2.0f;

			//triangle 1
			//bottom left corner
			aryTriangleList.Add(new CustomVertex.PositionNormalTextured(-fltSizeX, -fltSizeY, 0, 0,0,-1,0,0));

			//upper left corner
			aryTriangleList.Add(new CustomVertex.PositionNormalTextured(-fltSizeX, fltSizeY, 0, 0,0,-1,0,1));

			//upper right corner
			aryTriangleList.Add(new CustomVertex.PositionNormalTextured(fltSizeX, fltSizeY, 0, 0,0,-1,1,1));

			//triangle 2
			//bottom left corner
			aryTriangleList.Add(new CustomVertex.PositionNormalTextured(-fltSizeX, -fltSizeY, 0, 0,0,-1,0,0));

			//upper right corner
			aryTriangleList.Add(new CustomVertex.PositionNormalTextured(fltSizeX, fltSizeY, 0, 0,0,-1,1,1));

			//bottom right corner
			aryTriangleList.Add(new CustomVertex.PositionNormalTextured(fltSizeX, -fltSizeY, 0, 0,0,-1,1,0));

			return CreateMeshFromTriangleList(d3dDevice, aryTriangleList);
		}
	
		public static Mesh CreatePlane(Microsoft.DirectX.Direct3D.Device d3dDevice, float fltSideLength, int iTrianglesPerSide, float fltYPosition)
		{
			int i,j;
			float x, z, u, v, d_xz, d_uv; 
			float NY = 1, NZ = 0;

			ArrayList aryTriangleList = new ArrayList();

			d_xz = fltSideLength / iTrianglesPerSide;
			d_uv = 1.0f / (float)iTrianglesPerSide;

			v = 0.0f;
			z = (fltSideLength/2.0f);
			// Upper Plane 
			for( i=0; i < iTrianglesPerSide; i++ )
			{
				x = -(fltSideLength/2.0f);
				u = 0.0f;
				for( j=0; j < iTrianglesPerSide; j++ )
				{
					float Y0, Y1, Z0, Z1;

					Y0 = Y1 = fltYPosition;
					Z0 = z;
					Z1 = z - d_xz;

					// create square 
					aryTriangleList.Add(new CustomVertex.PositionNormalTextured(x, Y0, Z0, 0, NY, NZ, u, v));
					aryTriangleList.Add(new CustomVertex.PositionNormalTextured(x, Y1, Z1, 0, NY, NZ, u, v + d_uv));
					aryTriangleList.Add(new CustomVertex.PositionNormalTextured(x + d_xz, Y1, Z1, 0, NY, NZ, u + d_uv, v + d_uv));
					aryTriangleList.Add(new CustomVertex.PositionNormalTextured(x, Y0, Z0, 0, NY, NZ, u, v));
					aryTriangleList.Add(new CustomVertex.PositionNormalTextured(x + d_xz, Y1, Z1, 0, NY, NZ, u + d_uv, v + d_uv));
					aryTriangleList.Add(new CustomVertex.PositionNormalTextured(x + d_xz, Y0, Z0, 0, NY, NZ, u + d_uv, v));

					x += d_xz;
					u += d_uv;
				}
				z -= d_xz;
				v += d_uv;
			}
			
			return CreateMeshFromTriangleList(d3dDevice, aryTriangleList);
		}

		protected static void DumpVertex(CustomVertex.PositionNormalTextured vVertex)
		{Debug.WriteLine("(" + vVertex.X + ", " + vVertex.Y + ", " + vVertex.Z + ", " + vVertex.Nx + ", " + vVertex.Ny + ", " + vVertex.Nz + ")");}

		public static ArrayList ConvertTriStripToTriList(ArrayList aryStrips, bool bStartCCW)
		{return ConvertTriStripToTriList(aryStrips, bStartCCW, new CustomVertex.PositionNormalTextured(0,0,0,0,0,0,0,0));}

		public static ArrayList ConvertTriStripToTriList(ArrayList aryStrips, bool bStartCCW, object vType)
		{
			if(vType is CustomVertex.PositionNormalTextured)
				return ConvertTriStripToTriList_PNT(aryStrips, bStartCCW);
			else if(vType is CustomVertex.PositionNormalColored)
				return ConvertTriStripToTriList_PNC(aryStrips, bStartCCW);
			else if(vType is CustomVertex.PositionColored)
				return ConvertTriStripToTriList_PC(aryStrips, bStartCCW);
			else
				return null;
		}

		public static ArrayList ConvertTriStripToTriList_PNT(ArrayList aryStrips, bool bStartCCW)
		{
			ArrayList aryList = new ArrayList();
			Vector3[] vTempVerts = new Vector3[3];
			int i=0;
			Vector3 vNormal;

			CustomVertex.PositionNormalTextured vVertex;
			foreach(CustomVertex.PositionNormalTextured[] verts in aryStrips)
			{
				//				Debug.WriteLine("New Strip");

				// Now convert strip 'i' into triangles. 
				bool bCCW = bStartCCW;
				for(int iPos=0; iPos<verts.Length-2; iPos++)
				{
					if(bCCW)
					{
						for(int iVert=0; iVert<3; iVert++)
						{
							vVertex = verts[iPos+iVert];
							//							DumpVertex(vVertex);
							vTempVerts[iVert] = new Vector3(vVertex.X, vVertex.Y, vVertex.Z);
							aryList.Add(new CustomVertex.PositionNormalTextured(vVertex.X, vVertex.Y, vVertex.Z, vVertex.Nx, vVertex.Ny, vVertex.Nz, vVertex.Tu, vVertex.Tv));
						}
					}
					else
					{
						i=0;
						for(int iVert=2; iVert>=0; iVert--)
						{
							vVertex = verts[iPos+iVert];
							//							DumpVertex(vVertex);
							vTempVerts[i] = new Vector3(vVertex.X, vVertex.Y, vVertex.Z); i++;
							aryList.Add(new CustomVertex.PositionNormalTextured(vVertex.X, vVertex.Y, vVertex.Z, vVertex.Nx, vVertex.Ny, vVertex.Nz, vVertex.Tu, vVertex.Tv));
						}
					}

					vNormal =  Util_DX.CalculateNormal(vTempVerts);
					//					Debug.WriteLine("Normal: (" + vNormal.X + ", " + vNormal.Y + ", " + vNormal.Z + ")");
					//					Debug.WriteLine("");

					//If the normal is zero then we have tried to add a triangle face with 2 or more vertices that are the same.
					if(Math.Abs(vNormal.X) < 1e-9 && Math.Abs(vNormal.Y) < 1e-9 && Math.Abs(vNormal.Z) < 1e-9)
					{
						aryList.RemoveAt(aryList.Count-1);
						aryList.RemoveAt(aryList.Count-1);
						aryList.RemoveAt(aryList.Count-1);
					}
 
					//Swap the cull face direction
					bCCW = !bCCW;
				}
			}

			return aryList;
		}

		public static ArrayList ConvertTriStripToTriList_PNC(ArrayList aryStrips, bool bStartCCW)
		{
			ArrayList aryList = new ArrayList();
			Vector3[] vTempVerts = new Vector3[3];
			int i=0;

			CustomVertex.PositionNormalColored vVertex;
			foreach(CustomVertex.PositionNormalColored[] verts in aryStrips)
			{
				//				Debug.WriteLine("New Strip");

				// Now convert strip 'i' into triangles. 
				bool bCCW = bStartCCW;
				for(int iPos=0; iPos<verts.Length-2; iPos++)
				{
					if(bCCW)
					{
						for(int iVert=0; iVert<3; iVert++)
						{
							vVertex = verts[iPos+iVert];
							//							DumpVertex(vVertex);
							vTempVerts[iVert] = new Vector3(vVertex.X, vVertex.Y, vVertex.Z);
							aryList.Add(new CustomVertex.PositionNormalColored(vVertex.X, vVertex.Y, vVertex.Z, vVertex.Nx, vVertex.Ny, vVertex.Nz, vVertex.Color));
						}
					}
					else
					{
						i=0;
						for(int iVert=2; iVert>=0; iVert--)
						{
							vVertex = verts[iPos+iVert];
							//							DumpVertex(vVertex);
							vTempVerts[i] = new Vector3(vVertex.X, vVertex.Y, vVertex.Z); i++;
							aryList.Add(new CustomVertex.PositionNormalColored(vVertex.X, vVertex.Y, vVertex.Z, vVertex.Nx, vVertex.Ny, vVertex.Nz, vVertex.Color));
						}
					}
 
					//Swap the cull face direction
					bCCW = !bCCW;
				}
			}

			return aryList;
		}

		public static ArrayList ConvertTriStripToTriList_PC(ArrayList aryStrips, bool bStartCCW)
		{
			ArrayList aryList = new ArrayList();
			Vector3[] vTempVerts = new Vector3[3];
			int i=0;

			CustomVertex.PositionColored vVertex;
			foreach(CustomVertex.PositionColored[] verts in aryStrips)
			{
				//				Debug.WriteLine("New Strip");

				// Now convert strip 'i' into triangles. 
				bool bCCW = bStartCCW;
				for(int iPos=0; iPos<verts.Length-2; iPos++)
				{
					if(bCCW)
					{
						for(int iVert=0; iVert<3; iVert++)
						{
							vVertex = verts[iPos+iVert];
							//							DumpVertex(vVertex);
							vTempVerts[iVert] = new Vector3(vVertex.X, vVertex.Y, vVertex.Z);
							aryList.Add(new CustomVertex.PositionColored(vVertex.X, vVertex.Y, vVertex.Z, vVertex.Color));
						}
					}
					else
					{
						i=0;
						for(int iVert=2; iVert>=0; iVert--)
						{
							vVertex = verts[iPos+iVert];
							//							DumpVertex(vVertex);
							vTempVerts[i] = new Vector3(vVertex.X, vVertex.Y, vVertex.Z); i++;
							aryList.Add(new CustomVertex.PositionColored(vVertex.X, vVertex.Y, vVertex.Z, vVertex.Color));
						}
					}
 
					//Swap the cull face direction
					bCCW = !bCCW;
				}
			}

			return aryList;
		}

		public static void CreateVertexBufferFromTriangleList(VertexBuffer vBuffer, ArrayList aryList)
		{CreateVertexBufferFromTriangleList(vBuffer, aryList, new CustomVertex.PositionNormalTextured(0,0,0,0,0,0,0,0));}
			
		public static void CreateVertexBufferFromTriangleList(VertexBuffer vBuffer, ArrayList aryList, object vType)
		{
			if(vType is CustomVertex.PositionNormalTextured)
			{
				CustomVertex.PositionNormalTextured[] verts = new CustomVertex.PositionNormalTextured[aryList.Count];
				int iVertex = 0;

				foreach(CustomVertex.PositionNormalTextured vVertex in aryList)
				{
					verts[iVertex] = vVertex;
					iVertex++;
				}
				
				vBuffer.SetData(verts, 0, LockFlags.None);
			}
			else if(vType is CustomVertex.PositionTextured)
			{
				CustomVertex.PositionTextured[] verts = new CustomVertex.PositionTextured[aryList.Count];
				int iVertex = 0;

				foreach(CustomVertex.PositionTextured vVertex in aryList)
				{
					verts[iVertex] = vVertex;
					iVertex++;
				}
				
				vBuffer.SetData(verts, 0, LockFlags.None);
			}
		}


		public static Mesh CreateMeshFromTriangleList(Microsoft.DirectX.Direct3D.Device d3dDevice, ArrayList aryList)
		{return CreateMeshFromTriangleList(d3dDevice, aryList, new CustomVertex.PositionNormalTextured(0,0,0,0,0,0,0,0));}

		public static Mesh CreateMeshFromTriangleList(Microsoft.DirectX.Direct3D.Device d3dDevice, ArrayList aryList, object vType)
		{
			Mesh d3dMesh = null;
			
			if(vType is CustomVertex.PositionNormalTextured)
			{
				d3dMesh = new Mesh(aryList.Count/3, aryList.Count, MeshFlags.Managed, CustomVertex.PositionNormalTextured.Format, d3dDevice);

				using(VertexBuffer vb = d3dMesh.VertexBuffer)
				{
					GraphicsStream data = vb.Lock(0, 0, LockFlags.None);

					foreach(CustomVertex.PositionNormalTextured vVertex in aryList)
						data.Write(vVertex);

					vb.Unlock();
				}
			}
			else if(vType is CustomVertex.PositionTextured)
			{
				d3dMesh = new Mesh(aryList.Count/3, aryList.Count, MeshFlags.Managed, CustomVertex.PositionTextured.Format, d3dDevice);

				using(VertexBuffer vb = d3dMesh.VertexBuffer)
				{
					GraphicsStream data = vb.Lock(0, 0, LockFlags.None);

					foreach(CustomVertex.PositionTextured vVertex in aryList)
						data.Write(vVertex);

					vb.Unlock();
				}
			}
			else if(vType is CustomVertex.PositionNormalColored)
			{
				d3dMesh = new Mesh(aryList.Count/3, aryList.Count, MeshFlags.Managed, CustomVertex.PositionNormalColored.Format, d3dDevice);

				using(VertexBuffer vb = d3dMesh.VertexBuffer)
				{
					GraphicsStream data = vb.Lock(0, 0, LockFlags.None);

					foreach(CustomVertex.PositionNormalColored vVertex in aryList)
						data.Write(vVertex);

					vb.Unlock();
				}
			}
			else if(vType is CustomVertex.PositionColored)
			{
				d3dMesh = new Mesh(aryList.Count/3, aryList.Count, MeshFlags.Managed, CustomVertex.PositionColored.Format, d3dDevice);

				using(VertexBuffer vb = d3dMesh.VertexBuffer)
				{
					GraphicsStream data = vb.Lock(0, 0, LockFlags.None);

					foreach(CustomVertex.PositionColored vVertex in aryList)
						data.Write(vVertex);

					vb.Unlock();
				}
			}
			
			short[] aryIndices = new short[aryList.Count];
			for(short iIndex=0; iIndex<aryList.Count; iIndex++)
				aryIndices[iIndex] = iIndex;

			using (IndexBuffer ib = d3dMesh.IndexBuffer)
			{
				ib.SetData(aryIndices, 0, LockFlags.None);
			}

			int[] adjacency = new int[d3dMesh.NumberFaces * 3];
			d3dMesh.GenerateAdjacency(0.01F, adjacency); 
			d3dMesh.OptimizeInPlace(MeshFlags.OptimizeVertexCache, adjacency); 

			return d3dMesh;
		}

		#endregion 

		#region Receptive Field Generation



		#endregion 

		#region Math Methods

		public static bool MatricesEqual(Matrix m1, Matrix m2)
		{
			if(  m1.M11 != m2.M11 || m1.M12 != m2.M12 || m1.M13 != m2.M13 || m1.M14 != m2.M14 
				|| m1.M21 != m2.M21 || m1.M22 != m2.M22 || m1.M23 != m2.M23 || m1.M24 != m2.M24
				|| m1.M31 != m2.M31 || m1.M32 != m2.M32 || m1.M33 != m2.M33 || m1.M34 != m2.M34 
				|| m1.M41 != m2.M41 || m1.M42 != m2.M42 || m1.M43 != m2.M43 || m1.M44 != m2.M44)
				return false;
			else
				return true;
		}

		public static Microsoft.DirectX.Matrix CloneMatrix(Microsoft.DirectX.Matrix aryMatrix)
		{
			Microsoft.DirectX.Matrix aryNewMatrix = new Microsoft.DirectX.Matrix();
			aryNewMatrix.M11 = aryMatrix.M11;
			aryNewMatrix.M12 = aryMatrix.M12;
			aryNewMatrix.M13 = aryMatrix.M13;
			aryNewMatrix.M14 = aryMatrix.M14;

			aryNewMatrix.M21 = aryMatrix.M21;
			aryNewMatrix.M22 = aryMatrix.M22;
			aryNewMatrix.M23 = aryMatrix.M23;
			aryNewMatrix.M24 = aryMatrix.M24;

			aryNewMatrix.M31 = aryMatrix.M31;
			aryNewMatrix.M32 = aryMatrix.M32;
			aryNewMatrix.M33 = aryMatrix.M33;
			aryNewMatrix.M34 = aryMatrix.M34;

			aryNewMatrix.M41 = aryMatrix.M41;
			aryNewMatrix.M42 = aryMatrix.M42;
			aryNewMatrix.M43 = aryMatrix.M43;
			aryNewMatrix.M44 = aryMatrix.M44;

			return aryNewMatrix;
		}

		public static Vector3 Matrix3MultiplyVector(Matrix mat, Vector3 v)
		{
			Vector3 vNew = new Vector3();
			vNew.X = mat.M11 * v.X + mat.M21 * v.Y + mat.M31 * v.Z;
			vNew.Y = mat.M12 * v.X + mat.M22 * v.Y + mat.M32 * v.Z;
			vNew.Z = mat.M13 * v.X + mat.M23 * v.Y + mat.M33 * v.Z;
			return vNew;
		}

		public static Matrix Matrix3MakeRotationZ(float fltA)
		{
			float s = (float) Math.Sin(fltA);
			float c = (float) Math.Cos(fltA);

			Matrix m = new Matrix();

			m.M11 = c;
			m.M12 = -s;
			m.M13 = 0;

			m.M21 = s;
			m.M22 = c;
			m.M23 = 0;

			m.M31 = 0;
			m.M32 = 0;
			m.M33 = 1;

			return m;
		}

		public static double Distance(Vector3 vA, Vector3 vB)
		{
			return Math.Sqrt(Math.Pow(vA.X-vB.X, 2) + Math.Pow(vA.Y-vB.Y, 2) + Math.Pow(vA.Z-vB.Z, 2));
		}

		public static double Distance(Vector3 vA, float fltX, float fltY, float fltZ)
		{
			return Math.Sqrt(Math.Pow(vA.X-fltX, 2) + Math.Pow(vA.Y-fltY, 2) + Math.Pow(vA.Z-fltZ, 2));
		}

		public static double Distance(AnimatTools.Framework.Vec3d vA, float fltX, float fltY, float fltZ)
		{
			return Math.Sqrt(Math.Pow(vA.X-fltX, 2) + Math.Pow(vA.Y-fltY, 2) + Math.Pow(vA.Z-fltZ, 2));
		}

		public static bool CompareVectors(Vector3 v1, Vector3 v2)
		{
			if(Math.Abs(v1.X - v2.X) < 1e-4 &&
				Math.Abs(v1.Y - v2.Y) < 1e-4 &&
				Math.Abs(v1.Z - v2.Z) < 1e-4)
				return true;
			else
				return false;
		}

		public static bool CompareVectors(AnimatTools.Framework.Vec3d v1, AnimatTools.Framework.Vec3d v2)
		{
			if(Math.Abs(v1.X - v2.X) < 1e-4 &&
				Math.Abs(v1.Y - v2.Y) < 1e-4 &&
				Math.Abs(v1.Z - v2.Z) < 1e-4)
				return true;
			else
				return false;
		}

		public static bool CompareVectors(Vector3 v1, AnimatTools.Framework.Vec3d v2)
		{
			if(Math.Abs(v1.X - v2.X) < 1e-4 &&
				Math.Abs(v1.Y - v2.Y) < 1e-4 &&
				Math.Abs(v1.Z - v2.Z) < 1e-4)
				return true;
			else
				return false;
		}

		#endregion 

		#region James Stuff Orginaize Later
		/// <summary>
		/// Finds the third barycentric point given two barycentric points
		/// </summary>
		/// <param name="fB1">first barycentric point</param>
		/// <param name="fB2">second barycentric point</param>
		/// <returns>float</returns>
		public static float FindThirdBarycentricPoint(float fB1, float fB2)
		{
			return 1 - (fB1 + fB2);
		}
		/// <summary>
		/// Finds a point of intersection on a mesh
		/// </summary>
		/// <param name="mshMesh">Mesh to find the point on</param>
		/// <param name="iiInfo">Intersect Information of the mesh to find the point on</param>
		/// <returns>Vector3 representation of the point of intersection</returns>
		public static  Vector3 FindIntersectionPoint(Mesh mshMesh, IntersectInformation iiInfo)
		{
			//get barycentric points
			float fB1 = iiInfo.U;
			float fB2 = iiInfo.V;
			float fB3 = FindThirdBarycentricPoint(fB1, fB2);

			//get the intersected face
			Vector3[] v3Face = FindIntersectionFace(mshMesh, iiInfo);

			//convert the barycentric points to a 3d point
			Vector3 v3Point = (fB2 * v3Face[2]) + (fB1 * v3Face[1]) + (fB3 * v3Face[0]);

			return v3Point;			
		}
		/// <summary>
		/// Finds the face of a mesh that has been intersected with a ray
		/// </summary>
		/// <param name="iiIntInfo">IntersectInformation object from the intersection of a ray and face</param>
		/// <returns>Vector3[] of vertices that make up a triangle face</returns>
		public static Vector3[] FindIntersectionFace(Microsoft.DirectX.Direct3D.Mesh mshMesh, IntersectInformation iiInfo)
		{
			if(mshMesh != null)
			{
				//vertices that make up the face
				CustomVertex.PositionOnly[] vertsFace = new CustomVertex.PositionOnly[3];

				//	get the index buffer and lock it
				GraphicsStream buffer = mshMesh.LockIndexBuffer(LockFlags.ReadOnly);

				//	move to the position of the face index
				buffer.Position = iiInfo.FaceIndex * System.Runtime.InteropServices.Marshal.SizeOf(new IndexEntry());	//	6 = sizeof(IndexEntry)
				
				//	read the index entry from the buffer
				IndexEntry entry = (IndexEntry)buffer.Read(typeof(IndexEntry));

				//	we've got the info we need from the index buffer... unlock it
				mshMesh.UnlockIndexBuffer();
				buffer.Close();

				//now that we have the vertex locations, get each vertex from the vertex buffer

				//	get the vertex buffer and lock it
				buffer = mshMesh.LockVertexBuffer(LockFlags.ReadOnly);

				//	move to the first vertex position
				if(buffer.Length > entry.v1 * mshMesh.NumberBytesPerVertex)
				{
					buffer.Position = entry.v1 * mshMesh.NumberBytesPerVertex;
				
					//	grab first (left) vertex of triangle
					vertsFace[0] = (CustomVertex.PositionOnly)buffer.Read(typeof(CustomVertex.PositionOnly));
				}

				if(buffer.Length > entry.v2 * mshMesh.NumberBytesPerVertex)
				{
					// move to the second vertex position
					buffer.Position = entry.v2 * mshMesh.NumberBytesPerVertex;

					//	grab second (middle) vertex of triangle
					vertsFace[1] = (CustomVertex.PositionOnly)buffer.Read(typeof(CustomVertex.PositionOnly));
				}

				if(buffer.Length > entry.v3 * mshMesh.NumberBytesPerVertex)
				{
					// move to the third vertex of triangle
					buffer.Position = entry.v3 * mshMesh.NumberBytesPerVertex;

					//	grab last (right) vertex of triangle
					vertsFace[2] = (CustomVertex.PositionOnly)buffer.Read(typeof(CustomVertex.PositionOnly));
				}

				//unlock the buffer
				mshMesh.UnlockVertexBuffer();
				buffer.Close();
				
				// Vector representation instead of CustomVertex
				Vector3[] v3Verts = new Vector3[vertsFace.Length];

				v3Verts[0] = vertsFace[0].Position;
				v3Verts[1] = vertsFace[1].Position;
				v3Verts[2] = vertsFace[2].Position;

				return v3Verts;			
			}
			else
				return null;
		}
		
		/// <summary>
		/// Calculates the normal for a face
		/// </summary>
		/// <param name="vertsFace">Vertices that represent the face that has been intersected with </param>
		/// <returns>Vector3 object that is the normal of the face passed in</returns>
		public static Vector3 CalculateNormal(Vector3[] vertsFace)
		{
			//	calculate the directions of the left and right sides of the triangle
			Vector3 v3E1 = vertsFace[2] - vertsFace[1];
			Vector3 v3E2 = vertsFace[0] - vertsFace[2];

			//	calculate the cross product of the two triangle sides
			Vector3 v3Normal = Vector3.Cross(v3E1, v3E2);
			
			//  normalize the normal
			v3Normal.Normalize();			
			v3Normal.X = (float)Math.Round(v3Normal.X,5);
			v3Normal.Y = (float)Math.Round(v3Normal.Y,5);
			v3Normal.Z = (float)Math.Round(v3Normal.Z,5);
			
			return v3Normal;
		}

		
		public static bool TestIntersectWithMouse(Mesh mshMesh, object objViewport, Matrix mtxProjection, Matrix mtxView, Matrix mtxWorld, int iX, int iY, out IntersectInformation iiInfo)
		{
			// Create ray for intersection test
			Vector3	near = new Vector3(iX, iY, 0); 
			Vector3	far = new Vector3(iX, iY, 1);		

			//unproject the near and far vectors to 3D space
			near.Unproject(objViewport, mtxProjection, mtxView, mtxWorld);
			far.Unproject(objViewport, mtxProjection, mtxView, mtxWorld);
					
			//subtract the near vector from the far vector to get our ray
			far.Subtract(near);	

			return mshMesh.Intersect(near,far, out iiInfo);

		}

		public static Matrix RotateRightX(float angle)
		{			
			/*
			DX								RH
			1	0 		0					1	0		0
			0	Cos(x) 	Sin(x)				0	Cos(x)	-Sin(x)
			0	-Sin(x) Cos(x)				0	Sin(x)	Cos(x)
			*/		

			Matrix mRotX = Matrix.Identity;
			float fltCosX = (float)Math.Cos(angle);
			float fltSinX = (float)Math.Sin(angle);
			
			mRotX.M22 = fltCosX;
			mRotX.M23 = -fltSinX;
			mRotX.M32 = fltSinX;
			mRotX.M33 = fltCosX;

			Matrix dx = Matrix.RotationX(angle);

			return mRotX;
		}

		public static Matrix RotateRightY(float angle)
		{
			/*
			DX								RH
			Cos(y)	0 	-Sin(y)				Cos(y)	0	Sin(y)
			0		1 		0				0		1		0
			Sin(y)	0	Cos(y)				-Sin(y)	0	Cos(y)
			*/

			Matrix mRotY = Matrix.Identity;
			float fltCosY = (float)Math.Cos(angle);
			float fltSinY = (float)Math.Sin(angle);
			
			mRotY.M11 = fltCosY;
			mRotY.M13 = -fltSinY;
			mRotY.M31 = fltSinY;
			mRotY.M33 = fltCosY;

			Matrix dx = Matrix.RotationY(angle);

			return mRotY;
			
		}

		public static Matrix RotateRightZ(float angle)
		{			
			/*
			DX								RH
			Cos(z)	Sin(z) 	0				Cos(x)	-Sin(x)	0
			-Sin(z)	Cos(z) 	0				Sin(x)	Cos(x)	0
			0		0		1				0		0		1
			*/	

			Matrix mRotZ = Matrix.Identity;
			float fltCosZ = (float)Math.Cos(angle);
			float fltSinZ = (float)Math.Sin(angle);
			
			mRotZ.M11 = fltCosZ;
			mRotZ.M12 = -fltSinZ;
			mRotZ.M21 = fltSinZ;
			mRotZ.M22 = fltCosZ;

			Matrix dx = Matrix.RotationZ(angle);

			return mRotZ;
		}
		public static Vector3 DecomposeXYZRotationMatrix(Matrix mtxM)
		{
			Vector3 v3EularAngle = new Vector3();
			float m13 = -mtxM.M13;

			if(m13 > 1.0000f)
				m13 = 1;
			else if(m13 < -1.0000f)
				m13 = -1;

			v3EularAngle.Y = (float)Math.Asin(m13);

			float fThreshold = 0.001f;
			if(Math.Cos(v3EularAngle.Y) > fThreshold)
			{
				v3EularAngle.X = (float)Math.Atan2(mtxM.M23, mtxM.M33);
				v3EularAngle.Z = (float)Math.Atan2(mtxM.M12, mtxM.M11);
			}
			else
			{
				v3EularAngle.X = (float)Math.Atan2(mtxM.M21, mtxM.M22);
				v3EularAngle.Z = 0.0f;
			}
			return v3EularAngle;
		}
	
		
		#endregion
		
		#region Materials
		public static Material WhiteMaterial()
		{
			Material matM = new Material();

			matM.Diffuse = Color.White;
			matM.Ambient = Color.White;		

			return matM;
		}
		#endregion

		#region Matrix Loading/Saving
		public static void SaveMatrix(ref AnimatTools.Interfaces.StdXml oXml, string strName,  Matrix mtxM)
		{			
			string str = mtxM.M11.ToString() +"," + mtxM.M12.ToString() +"," + mtxM.M13.ToString() +"," + mtxM.M14.ToString() +","; 
			str += mtxM.M21.ToString() +"," + mtxM.M22.ToString() +"," + mtxM.M23.ToString() +"," + mtxM.M24.ToString() +","; 
			str += mtxM.M31.ToString() +"," + mtxM.M32.ToString() +"," + mtxM.M33.ToString() +"," + mtxM.M34.ToString() +","; 
			str += mtxM.M41.ToString() +"," + mtxM.M42.ToString() +"," + mtxM.M43.ToString() +"," + mtxM.M44.ToString();

			oXml.AddChildElement(strName,str);	
		}
		public static Matrix LoadMatrix(ref AnimatTools.Interfaces.StdXml oXml, string strName)
		{
			string str = oXml.GetChildString(strName);
			string[] strMtxElem = str.Split(',');

			Matrix mtx = new Matrix();
			mtx.M11 = float.Parse(strMtxElem[0]);
			mtx.M12 = float.Parse(strMtxElem[1]);
			mtx.M13 = float.Parse(strMtxElem[2]);
			mtx.M14 = float.Parse(strMtxElem[3]);

			mtx.M21 = float.Parse(strMtxElem[4]);
			mtx.M22 = float.Parse(strMtxElem[5]);
			mtx.M23 = float.Parse(strMtxElem[6]);
			mtx.M24 = float.Parse(strMtxElem[7]);

			mtx.M31 = float.Parse(strMtxElem[8]);
			mtx.M32 = float.Parse(strMtxElem[9]);
			mtx.M33 = float.Parse(strMtxElem[10]);
			mtx.M34 = float.Parse(strMtxElem[11]);

			mtx.M41 = float.Parse(strMtxElem[12]);
			mtx.M42 = float.Parse(strMtxElem[13]);
			mtx.M43 = float.Parse(strMtxElem[14]);
			mtx.M44 = float.Parse(strMtxElem[15]);

			return mtx;
		}

		#endregion

		#region Conversions
		public static Vector3 Cartesian_To_Spherical(Vector3 v3Coord)
		{
			float r = (float)(Math.Sqrt((v3Coord.X * v3Coord.X) + (v3Coord.Y * v3Coord.Y) + (v3Coord.Z * v3Coord.Z)));
			float s = (float)Math.Sqrt((v3Coord.X * v3Coord.X) + (v3Coord.Y * v3Coord.Y));
			
			float phi = (float)(Math.Acos(v3Coord.Z / r));
//
//			float theta = 0.0f;
//			if(v3Coord.X >= 0)
//				theta = (float)(Math.Asin(v3Coord.Y / s));
//			else
//				theta = (float)(Math.PI - Math.Asin(v3Coord.Y / s));
				
			//theta = (float)(Math.Atan(v3Coord.Y / v3Coord.X));

			float theta = (float)(Math.Atan2(v3Coord.Y, v3Coord.X));
			
			

			return new Vector3(r, theta, phi);

			
		}

		public static Vector3 Spherical_To_Cartesian(Vector3 v3Coord)
		{

			float x = (float)(v3Coord.X * Math.Cos(v3Coord.Y) * Math.Sin(v3Coord.Z)); 
			float y = (float)(v3Coord.X * Math.Sin(v3Coord.Y) * Math.Sin(v3Coord.Z));
			float z = (float)(v3Coord.X * Math.Cos(v3Coord.Z));
			
			return new Vector3(x, y, z);			

		}
		#endregion

	}
}
