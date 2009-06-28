using System;
using System.Collections;
using System.IO;
using System.Diagnostics;
using System.Drawing;

using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace VortexAnimatTools.DataObjects
{
	/// <summary>
	/// Loads an Wavefront OBJ file into a DirectX Mesh
	/// </summary>
	public class OBJLoader
	{	
		private ArrayList m_alVertices = new ArrayList();	//holder for the vertices in the file
		private ArrayList m_alVertFormat = new ArrayList();	//holder for the format of the faces
		private ArrayList m_alTextCoords = new ArrayList();	//holder for the texture coordinates in the file
		private ArrayList m_alNormals = new ArrayList();	//holder for the normals in the file

		private int m_intNumFaces = 0;
		private int m_intNumVerts = 0;

		private Mesh m_d3dMesh = null;
		private Material m_d3dMat = new Material();

		private Vector3 m_v3LowerLeftExtent = new Vector3(0,0,0);
		private Vector3 m_v3UpperRightExtent = new Vector3(0,0,0);

		private bool m_bLoaded = false;

		private string m_strName = "";

		/// <summary>
		/// Returns the loaded OBJ mesh
		/// </summary>
		public Mesh Mesh
		{
			get{return m_d3dMesh;}
		}

		/// <summary>
		/// Returns the material associated with this mesh.  If
		/// no material is found a default white material
		/// will be used
		/// </summary>
		public Material Material
		{
			get{return m_d3dMat;}
		}

		/// <summary>
		/// Returns the lower left extent of the mesh for
		/// bounding box purposes
		/// </summary>
		public Vector3 LowerLeftExtent
		{
			get{return m_v3LowerLeftExtent;}
		}

		/// <summary>
		/// Returns the upper right extent of the mesh for
		/// bounding box purposes
		/// </summary>
		public Vector3 UpperRightExtent
		{
			get{return m_v3UpperRightExtent;}
		}

		
		/// <summary>
		/// Returns true if the mesh was successfully loaded
		/// </summary>
		public bool IsLoaded
		{
			get{return m_bLoaded;}
		}

		/// <summary>
		/// Returns the name of the mesh
		/// </summary>
		public string Name
		{
			get{return m_strName;}
		}


		/// <summary>
		/// Creates a new Wavefront OBJ loader.  Will try to load
		/// the mesh from the filename and the material associated
		/// with the obj.
		/// </summary>
		/// <param name="strFile"></param>
		public OBJLoader(string strFile, Device d3dDevice)
		{
			
				//make sure the file exists
				if(File.Exists(strFile))
				{
					//start reading the file
					StreamReader sr = new StreamReader(strFile);

					try
					{						
						string strLine = null;

						while ((strLine = sr.ReadLine()) != null) 
						{
							if(strLine.StartsWith("o "))
							{
								//found the name of the mesh
								m_strName = strLine.Substring(2);
							}
							else if(strLine.StartsWith("mtllib "))
							{
								//found a material file.
								string strMat = strFile.Substring(0, strFile.LastIndexOf("\\") + 1) + strLine.Substring(7);

								LoadMaterial(strMat);
							}
							else if (strLine.StartsWith("v "))
							{
								//found a vertex
								ProcessVertex(strLine);
							}
							else if(strLine.StartsWith("vn "))
							{
								//found a normal
								ProcessNormal(strLine);
							}
							else if(strLine.StartsWith("vt "))
							{
								//found a texture coordinate
								ProcessTextureCoordinate(strLine);
							}
							else if(strLine.StartsWith("f "))
							{
								//found a face
								ProcessFace(strLine);
							}

						}//end while

						GenerateMesh(d3dDevice);

						
					}
					catch(Exception ex)
					{
						AnimatTools.Framework.Util.DisplayError(ex);
					}
					finally
					{
						sr.Close();
					}
					
				}//end if file exists;
			
		}

		/// <summary>
		/// Parse a vertex from the line.
		/// Expected format:  v x y z
		/// </summary>
		/// <param name="strLine">a line from the obj file</param>
		private void ProcessVertex(string strLine)
		{
			string[] strVert = strLine.Split(' ');			
			if(strVert.Length != 4)
				throw new Exception(strLine + "\nInvalid vertex format in OBJ file");
			else
			{
				//parse the float values
				float x = float.Parse(strVert[1]);
				float y = float.Parse(strVert[2]);
				float z = float.Parse(strVert[3]);

				//add the vertex to the list
				m_alVertices.Add(new Vector3(x, y, z));

				m_intNumVerts++;

				if(x < m_v3LowerLeftExtent.X)
					m_v3LowerLeftExtent.X = x;
				if(x > m_v3UpperRightExtent.X)
					m_v3UpperRightExtent.X = x;

				if(y < m_v3LowerLeftExtent.Y)
					m_v3LowerLeftExtent.Y = y;
				if(y > m_v3UpperRightExtent.Y)
					m_v3UpperRightExtent.Y = y;

				if(z < m_v3LowerLeftExtent.Z)
					m_v3LowerLeftExtent.Z = z;
				if(z > m_v3UpperRightExtent.Z)
					m_v3UpperRightExtent.Z = z;


			}
		}

		/// <summary>
		/// Parse a normal from the line.
		/// Expected format:  vn x y z
		/// </summary>
		/// <param name="strLine">a line from the obj file</param>
		private void ProcessNormal(string strLine)
		{
			string[] strNormal = strLine.Split(' ');
			if(strNormal.Length != 4)
				throw new Exception(strLine + "\nInvalid normal format in OBJ file");
			else
			{
				//parse the float values
				float x = float.Parse(strNormal[1]);
				float y = float.Parse(strNormal[2]);
				float z = float.Parse(strNormal[3]);

				//add the vertex to the list
				m_alNormals.Add(new Vector3(x, y, z));
			}
		}

		/// <summary>
		/// Parse a texture coordinate from the line.
		/// Expected format:  vt x y z
		/// </summary>
		/// <param name="strLine">a line from the obj file</param>
		private void ProcessTextureCoordinate(string strLine)
		{
			string[] strTextCoord = strLine.Split(' ');
			if(strTextCoord.Length == 4)
			{
				//parse the float values
				float x = float.Parse(strTextCoord[1]);
				float y = float.Parse(strTextCoord[2]);
				float z = float.Parse(strTextCoord[3]);

				//add the vertex to the list
				m_alTextCoords.Add(new Vector3(x, y, z) );
			}
			else if(strTextCoord.Length == 3)
			{
				//parse the float values
				float x = float.Parse(strTextCoord[1]);
				float y = float.Parse(strTextCoord[2]);

				//add the vertex to the list
				m_alTextCoords.Add(new Vector3(x, y, 0) );
			}
			else
				throw new Exception(strLine + "\nInvalid texture coordinate format in OBJ file");

		}

		/// <summary>
		/// Parse a texture coordinate from the line.
		/// Expected format:  f v1//vn1 v2//vn2 v3//vn3 OR
		///					  f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
		///					  
		///	v = index into vertex array
		///	vt = index into text coord array
		///	vn = index into normal array
		/// </summary>
		/// <param name="strLine">a line from the obj file</param>
		private void ProcessFace(string strLine)
		{
			string[] strVertFormat = strLine.Split(' ');
			if(strVertFormat.Length != 4 && strVertFormat.Length !=  5)
				throw new Exception(strLine + "\r\nInvalid face format in OBJ file: " + m_strName);
			else
			{				
				//replace the //'s with /
				strLine = strLine.Replace("//", "/");
				strVertFormat = strLine.Split(' ');

				m_alVertFormat.Add(strVertFormat[1].Split('/'));
				m_alVertFormat.Add(strVertFormat[2].Split('/'));
				m_alVertFormat.Add(strVertFormat[3].Split('/'));

				m_intNumFaces++;

				if(strVertFormat.Length == 5)
				{
					m_alVertFormat.Add(strVertFormat[3].Split('/'));
					m_alVertFormat.Add(strVertFormat[4].Split('/'));
					m_alVertFormat.Add(strVertFormat[1].Split('/'));

					m_intNumFaces++;
				}
			}
		}

		/// <summary>
		/// Generates a mesh from the parsed 
		/// information of the file
		/// </summary>
		private void GenerateMesh(Device d3dDevice)
		{
			if(m_alNormals.Count == 0)
				throw new System.Exception("No normals were found for this mesh.");

			if(m_intNumFaces == 0)
				throw new System.Exception("No faces were found for this mesh.");

			if(m_intNumVerts == 0)
				throw new System.Exception("No vertices were found for this mesh.");
			
			//create a mesh with Poisiton, Normal, and Texture info.  Even if it doesn't contain TextCoords
			m_d3dMesh = new Mesh(m_intNumFaces, m_intNumVerts, MeshFlags.Managed, CustomVertex.PositionNormalTextured.Format, d3dDevice);

			//index array
			short[] aryIndices = new short[m_alVertFormat.Count];
			CustomVertex.PositionNormalTextured[] aryVerts = new CustomVertex.PositionNormalTextured[m_intNumVerts];

			Vector3 v, t, n;
			CustomVertex.PositionNormalTextured cvVert;

			//loop through each face and apply the format
			for(int i=0; i<m_intNumFaces * 3; i++)
			{
				//parse the vertex information
				string[] aryVertInfo = (string[])m_alVertFormat[i];

				//first one is vertex index
				short index = short.Parse(aryVertInfo[0]);

				//OBJ format starts at 1 not 0
				index--;

				//set the index arry
				aryIndices[i] = index;	
				v = (Vector3)m_alVertices[index];
				

				t = new Vector3(0,0,0);
				//parse the texture coords
				if( aryVertInfo.Length == 3)
				{
					index = short.Parse(aryVertInfo[1]);
					index--;

					//set the texture coordinate
					t = (Vector3)m_alTextCoords[index];					

					index = short.Parse(aryVertInfo[2]);
					index--;

					//set the normal
					n = (Vector3)m_alNormals[index];
				}
				else
				{
					index = short.Parse(aryVertInfo[1]);
					index--;

					//set the normal
					n = (Vector3)m_alNormals[index];									
				}

				cvVert = aryVerts[aryIndices[i]];					
					
				cvVert.Position = v;	
			
				cvVert.Normal = n;

				cvVert.Tu = t.X;
				cvVert.Tv = t.Y;

				aryVerts[aryIndices[i]] = cvVert;

			}//end for loop

			m_d3dMesh.VertexBuffer.SetData(aryVerts,0, LockFlags.None);
			m_d3dMesh.IndexBuffer.SetData(aryIndices,0,LockFlags.None);

			AttributeRange ar = new AttributeRange();
			ar.AttributeId = 0;
			ar.FaceCount = m_intNumFaces;
			ar.FaceStart = 0;
			ar.VertexCount = m_intNumVerts;
			ar.VertexStart = 0;

			m_d3dMesh.SetAttributeTable(new AttributeRange[] {ar});
			
			int[] adj = new int[m_intNumFaces * 3];
			m_d3dMesh.GenerateAdjacency(0.01f, adj);
			m_d3dMesh.OptimizeInPlace(MeshFlags.OptimizeVertexCache | MeshFlags.OptimizeIgnoreVerts, adj);
			m_d3dMesh.ComputeNormals();

			m_bLoaded = true;
		}

		private void LoadMaterial(string strMatFile)
		{
			//make sure the file exists
			if(File.Exists(strMatFile))
			{
				//start reading the file
				StreamReader sr = new StreamReader(strMatFile);
					
				try
				{
					string strLine = null;

					while ((strLine = sr.ReadLine()) != null) 
					{
						if(strLine.StartsWith("Ka "))
						{
							m_d3dMat.Ambient = ParseColor(strLine.Substring(3));
						}
						else if(strLine.StartsWith("Kd "))
						{
							m_d3dMat.Diffuse = ParseColor(strLine.Substring(3));
						}
						else if(strLine.StartsWith("Ks "))
						{
							m_d3dMat.Specular = ParseColor(strLine.Substring(3));
						}
						else if(strLine.StartsWith("Ns "))
						{
							m_d3dMat.SpecularSharpness = float.Parse(strLine.Substring(3));
						}					
					
					}//end while
				}
				catch(Exception ex)
				{
					AnimatTools.Framework.Util.DisplayError(ex);
				}
				finally
				{
					sr.Close();
				}
			}//end if file exists
			else
			{
				m_d3dMat.Diffuse = Color.White;
			}
		}

		/// <summary>
		/// Parses a color from a string representation of rgb
		/// </summary>
		/// <param name="strColor">string representation of rgb in float format</param>
		/// <returns>The color</returns>
		private Color ParseColor(string strColor)
		{
			string[] strRGB = strColor.Split(' ');

			if(strRGB.Length != 3)
				throw new Exception("Invalid color format in MTL file");
			else
			{
				int r = (int)(float.Parse(strRGB[0]) * 255);
				int g = (int)(float.Parse(strRGB[1]) * 255);
				int b = (int)(float.Parse(strRGB[2]) * 255);

				return Color.FromArgb(r,g,b);
			}
		}		
	}
}
