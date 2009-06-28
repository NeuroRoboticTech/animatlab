#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;
using namespace System::Runtime::InteropServices;
#using <mscorlib.dll>

 
namespace AnimatTools
{
	namespace Interfaces
	{
 
		void UpdateDataCallback(void *lpSim);
		void StartSimulationCallback(void *lpSim);
		void PauseSimulationCallback(void *lpSim);
		void EndingSimulationCallback(void *lpSim);

		/// <summary> 
		/// Summary for SimulatorInterface
		/// </summary>
		__gc public class SimulatorInterface :  public System::ComponentModel::Component
		{
		public:
			SimulatorInterface(void)
			{
				InitializeComponent();
				m_lpSim = NULL;
				m_bPaused = true;
				m_bIsLoaded = false;
			}

			SimulatorInterface(System::ComponentModel::IContainer *container) : components(0)
			{
				/// <summary>
				/// Required for Windows.Forms Class Composition Designer support
				/// </summary>

				container->Add(this);
				InitializeComponent();
			}

			~SimulatorInterface()
			{
				//if(m_iInstanceID>-1) m_arySimulators->RemoveAt(m_iInstanceID);
				if(m_lpSim) delete m_lpSim;
			}

			void FireUpdateDataEvent()    
			{
				OnUpdateData();
			}

			void FireStartSimulationEvent()    
			{
				OnStartSimulation();
			}

			void FirePauseSimulationEvent()    
			{
				OnPauseSimulation();
			}

			void FireEndingSimulationEvent()    
			{
				OnEndingSimulation();
			}

			void FireResetSimulationEvent()    
			{
				OnResetSimulation();
			}

			void FireSimulationErrorEvent()    
			{
				OnSimulationError();
			}

			//static Object* m_arySimulators __gc[] = new Array;
			//int m_iInstanceID;

			__event void OnUpdateData();
			__event void OnStartSimulation();
			__event void OnPauseSimulation();
			__event void OnEndingSimulation();
			__event void OnResetSimulation();
			__event void OnSimulationError();

			__property void set_Logger(AnimatTools::Interfaces::Logger *lpLogger)
			{m_lpLogger = lpLogger;}

			__property AnimatTools::Interfaces::Logger *get_Logger()
			{return m_lpLogger;}

			long CurrentMillisecond()
			{
				if(m_lpSim) 
					return m_lpSim->Millisecond();
				else
					return 0;
			}

			bool Paused()
			{
				if(m_lpSim)
					return m_lpSim->Paused();
				else
					return true;
			}

			System::Boolean Loaded()
			{
				return m_bIsLoaded;
			}

			__property float get_PlaybackRate()
			{
				if(m_lpSim) 
					return m_lpSim->PlaybackRate();
				else
					return 1;
			}

			__property void set_PlaybackRate(float fltVal)
			{
				try
				{
					if(m_lpSim) 
						m_lpSim->PlaybackRate(fltVal);
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to create the simulation.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to create the simulation.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			__property System::Boolean get_UseAlphaBlending()
			{
				if(m_lpSim) 
					return m_lpSim->UseAlphaBlending();
				else
					return TRUE;
			}
			__property void set_UseAlphaBlending(System::Boolean bVal)
			{
				try
				{
					if(m_lpSim) 
					{
						if(bVal)
							m_lpSim->UseAlphaBlending(TRUE);
						else
							m_lpSim->UseAlphaBlending(FALSE);
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to set UseAlphaBlending.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to set UseAlphaBlending.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void CreateSimulation(System::String *sXml, System::String *sLogFile)
			{
				try
				{
					LogMsg(Logger::enumLogLevel::Info, "Starting CreateSimulation");

					CStdXml oXml;
					oXml.Deserialize(Util::StringToStd(sXml));
					
					if(m_newThread)
						throw new System::Exception("A thread is already running. You can not create a new simulation while one is currently running.");

					if(m_lpSim)
					{
						delete m_lpSim;
						m_lpSim = NULL;
					}

					LogMsg(Logger::enumLogLevel::Debug, "About to create the simulation.");
					m_lpSim = AnimatLibrary::Simulator::CreateSimulator(oXml);

					//string strLogFile = Util::StringToStd(sLogFile);
					//Std_SetLogFilePrefix(strLogFile);

					LogMsg(Logger::enumLogLevel::Debug, "About to load the simulation.");
					m_lpSim->Load(oXml);
					m_lpSim->Paused(true);
					m_bIsLoaded = true;

					LogMsg(Logger::enumLogLevel::Info, "Finished CreateSimulation");
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to create the simulation.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to create the simulation.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void Simulate(IntPtr hParentWnd, bool bPaused)
			{
				try
				{
					LogMsg(Logger::enumLogLevel::Info, "Starting Simulate");

					//if(strProjectFile->Length() == 0)
					//	throw new System::Exception("No project file was specified for the simulator to run.");
					if(!m_lpSim)
						throw new System::Exception("You must first create a simulation before you can run it.");

					m_bPaused = bPaused;
					m_hParentWnd = hParentWnd;
					System::Threading::ThreadStart *startThread = new System::Threading::ThreadStart(this, &SimulatorInterface::RunSimulator);
					m_newThread = new System::Threading::Thread(startThread);
					m_newThread->Start();

					LogMsg(Logger::enumLogLevel::Info, "Finished Simulate");
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to start the simulation.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to start the simulation.";
					throw new System::Exception(m_strErrorMessage);
				}

			}

			void ShutdownSimulation()
			{
				try
				{
					if(m_lpSim) 
						m_lpSim->ShutdownSimulation();

					if(m_newThread)
						m_newThread->Join();
					m_newThread = NULL;
					m_bIsLoaded = false;
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to shutdown the simulation.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to shutdown the simulation.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			bool StartSimulation()
			{
				try
				{
					bool bVal = false;

					if(m_lpSim) 
						bVal = m_lpSim->StartSimulation();

					return bVal;
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to start the simulation.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to start the simulation.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			bool PauseSimulation()
			{
				try
				{
					if(m_lpSim) 
						return m_lpSim->PauseSimulation();

					return false;
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to pause the simulation.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to pause the simulation.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			System::String *ErrorMessage()
			{return m_strErrorMessage;}

			void TrackCamera(System::Boolean bTrackCamera, String *sLookAtStructureID, String *sLookAtBodyID)
			{
				try
				{
					if(m_lpSim) 
					{
						BOOL bVal = bTrackCamera;
						string strLookAtStructureID = Util::StringToStd(sLookAtStructureID);
						string strLookAtBodyID = Util::StringToStd(sLookAtBodyID);

						m_lpSim->TrackCamera(bTrackCamera, strLookAtStructureID, strLookAtBodyID);
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to call TrackCamera.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to call TrackCamera.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			String *AddKeyFrame(String *strType, long lStartMillisecond, long lEndMillisecond)
			{
				try
				{
					if(m_lpSim) 
					{
						string strSType = Util::StringToStd(strType);
						long lStart = m_lpSim->MillisecondToSlice(lStartMillisecond);
						long lEnd = m_lpSim->MillisecondToSlice(lEndMillisecond);

						return m_lpSim->AddKeyFrame(strSType, lStart, lEnd).c_str();
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to add the keyframe.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
					return "";
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to add a keyframe.";
					throw new System::Exception(m_strErrorMessage);
					return "";
				}
			}

			void RemoveKeyFrame(String *strID)
			{
				try
				{
					if(m_lpSim) 
					{
						string strSID = Util::StringToStd(strID);

						m_lpSim->RemoveKeyFrame(strSID);
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to add the keyframe.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to remove a keyframe.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			String *MoveKeyFrame(String *strID, long lStartMillisecond, long lEndMillisecond)
			{
				try
				{
					if(m_lpSim) 
					{
						string strSID = Util::StringToStd(strID);
						long lStart = m_lpSim->MillisecondToSlice(lStartMillisecond);
						long lEnd = m_lpSim->MillisecondToSlice(lEndMillisecond);

						return m_lpSim->MoveKeyFrame(strSID, lStart, lEnd).c_str();
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to add the keyframe.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to move a keyframe.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void EnableVideoPlayback(String *strID)
			{
				try
				{
					if(m_lpSim) 
					{
						string strSID = Util::StringToStd(strID);
						m_lpSim->EnableVideoPlayback(strSID);
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while enabling video playback for the keyframe.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while enabling video playback for the video for the keyframe.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void DisableVideoPlayback()
			{
				try
				{
					if(m_lpSim) 
						m_lpSim->DisableVideoPlayback();
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to disable video playback for the keyframe.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to disable video playback for the keyframe.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void StartVideoPlayback()
			{
				try
				{
					if(m_lpSim) 
						m_lpSim->StartVideoPlayback();
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to play the video for the keyframe.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to play the video for the keyframe.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void StopVideoPlayback()
			{
				try
				{
					if(m_lpSim) 
						m_lpSim->StopVideoPlayback();
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to stop playing the video for the keyframe.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to stop playing the video for the keyframe.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void StepVideoPlayback(int iFrameCount)
			{
				try
				{
					if(m_lpSim) 
						m_lpSim->StepVideoPlayback(iFrameCount);
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to step the playback of the video for the keyframe.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to step the playback of the video for the keyframe.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void MoveSimulationToKeyFrame(String *strID)
			{
				try
				{
					if(m_lpSim) 
					{
						string strSID = Util::StringToStd(strID);
						m_lpSim->MoveSimulationToKeyFrame(strSID);
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to add the keyframe.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to move simulation to a keyframe.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void SaveVideo(String *strPath)
			{
				try
				{
					if(m_lpSim) 
					{
						string strSPath = Util::StringToStd(strPath);
						m_lpSim->SaveVideo(strSPath);
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to add the keyframe.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to save video.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			System::Boolean FindDataChart(String *strKey, System::Boolean bThrowError)
			{
				try
				{
					if(m_lpSim) 
					{
						string strSKey = Util::StringToStd(strKey);
						
						AnimatLibrary::Charting::DataChartMgr *lpChartMgr = m_lpSim->DataChartMgr();
						if(lpChartMgr->Find(strSKey, FALSE))
							return true;
						else
							return false;
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to find a data chart.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to find a data chart.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void RemoveDataChart(String *strKey)
			{
				try
				{
					if(m_lpSim) 
					{
						string strSKey = Util::StringToStd(strKey);

						AnimatLibrary::Charting::DataChartMgr *lpChartMgr = m_lpSim->DataChartMgr();
						lpChartMgr->Remove(m_lpSim, strSKey);
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to remove a data chart.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to remove a data chart.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void ModifyDataChart(String *strKey, float fltCollectWindow, float fltCollectInterval)
			{
				try
				{
					if(m_lpSim) 
					{
						string strSKey = Util::StringToStd(strKey);

						//Lets calculate the number of slices for the collect interval.
						int iCollectInterval = (int) (fltCollectInterval/m_lpSim->TimeStep());
						if(iCollectInterval<=0) iCollectInterval = 1;

						AnimatLibrary::Charting::DataChartMgr *lpChartMgr = m_lpSim->DataChartMgr();
						AnimatLibrary::Charting::DataChart *lpChart = dynamic_cast<AnimatLibrary::Charting::DataChart *>(lpChartMgr->Find(strSKey));

						lpChart->EndTime(fltCollectWindow);
						lpChart->CollectInterval(iCollectInterval);
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to remove a data chart.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to remove a data chart.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void AddDataChart(String *sModuleName, String *sType, String *sXml)
			{
				AnimatLibrary::Charting::DataChart *lpChart = NULL;

				try
				{
					if(m_lpSim) 
					{
						string strModuleName = Util::StringToStd(sModuleName);
						string strType = Util::StringToStd(sType);
						string strXml = Util::StringToStd(sXml);

						lpChart = dynamic_cast<AnimatLibrary::Charting::DataChart *>(m_lpSim->CreateObject(strModuleName, "DataChart", strType));

						CStdXml oXml;
						oXml.Deserialize(strXml);

						lpChart->LoadChart(m_lpSim, oXml);

						AnimatLibrary::Charting::DataChartMgr *lpChartMgr = m_lpSim->DataChartMgr();
						lpChartMgr->Add(m_lpSim, lpChart);
					}
				}
				catch(CStdErrorInfo oError)
				{
					if(lpChart) delete lpChart;
					string strError = "An error occurred while attempting to add a data chart.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					if(lpChart) delete lpChart;
					m_strErrorMessage = "An unknown error occurred while attempting to add a data chart.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void ReInitializeSimulation()
			{
				try
				{
					if(m_lpSim) 
					{
						AnimatLibrary::Charting::DataChartMgr *lpChartMgr = m_lpSim->DataChartMgr();
						lpChartMgr->ReInitialize(m_lpSim);

						AnimatLibrary::ExternalStimuli::ExternalStimuliMgr *lpStimMgr = m_lpSim->ExternalStimuliMgr();
						lpStimMgr->ReInitialize(m_lpSim);
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to re-initialize the data charts.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to re-initialize the data charts.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			System::Int32 RetrieveChartData(String *sChartKey, System::Single (*aryTimeData) __gc[,], System::Single (*aryData) __gc[,])
			{
				try
				{
					System::Int32 iRowCount=0;

					if(m_lpSim) 
					{
						string strChartKey = Util::StringToStd(sChartKey);

						AnimatLibrary::Charting::DataChartMgr *lpChartMgr = m_lpSim->DataChartMgr();
						AnimatLibrary::Charting::DataChart *lpChart = dynamic_cast<AnimatLibrary::Charting::DataChart *>(lpChartMgr->Find(strChartKey));

						if(lpChart->Lock())
						{
							float *lpDataBuffer = lpChart->DataBuffer();
							float *lpTimeBuffer = lpChart->TimeBuffer();
							long lColCount = lpChart->ColumnCount();
							long lCurrentRow = lpChart->CurrentRow();
							//if(lCurrentRow > 10000) lCurrentRow = 10000;
							long lBufferSize = lColCount * lCurrentRow;

							if(lCurrentRow > 0)
							{
								//First lets create the arrays.
								*aryTimeData = new System::Single __gc[lColCount, lCurrentRow];
								*aryData = new System::Single __gc[lColCount, lCurrentRow];
								//System::Single aryTempData __gc[]= new System::Single __gc[lColCount];
								//System::Single *aryTest = &(*aryYData[0]);
								
								//Unfortuanetly, I have been completely unable to find a way to use Marshal::Copy with a 2D array.
								//So I will have to just manually copy the stupid thing by hand. I know this is much slower, but I 
								//have been unable to find another alternative.
								long lCol=0;
								if(lpTimeBuffer)
								{
									for(long lRow=0; lRow<lCurrentRow; lRow++)
									{
										for(lCol=0; lCol<lColCount; lCol++)
										{
											(*aryTimeData)[lCol, lRow] = lpTimeBuffer[lRow];
											(*aryData)[lCol, lRow] = lpDataBuffer[(lRow*lColCount) + lCol];
										}
									}
								}
								else
								{
									for(long lRow=0; lRow<lCurrentRow; lRow++)
									{
										for(lCol=0; lCol<lColCount; lCol++)
											(*aryData)[lCol, lRow] = lpDataBuffer[(lRow*lColCount) + lCol];
									}
								}

								//System::Runtime::InteropServices::Marshal::Copy(IntPtr((void *)lpBuffer), aryYData, 0, lBufferSize);

								//Reset the current row to the first one.
								lpChart->CurrentRow(0);
								iRowCount = lCurrentRow;
							}

							lpChart->Unlock();
						}
					}

					return iRowCount;
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to re-initialize the data charts.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to re-initialize the data charts.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			System::Boolean FindDataColumn(String *sChartKey, String *sColumnName, System::Boolean bThrowError)
			{
				try
				{
					if(m_lpSim) 
					{
						string strChartKey = Util::StringToStd(sChartKey);
						string strColumnName = Util::StringToStd(sColumnName);
						
						AnimatLibrary::Charting::DataChartMgr *lpChartMgr = m_lpSim->DataChartMgr();
						if(lpChartMgr->FindDataColumn(strChartKey, strColumnName, FALSE))
							return true;
						else
							return false;
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to find a data column.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to find a data column.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void RemoveDataColumn(String *sChartKey, String *sColumnName)
			{
				try
				{
					if(m_lpSim) 
					{
						string strChartKey = Util::StringToStd(sChartKey);
						string strColumnName = Util::StringToStd(sColumnName);

						AnimatLibrary::Charting::DataChartMgr *lpChartMgr = m_lpSim->DataChartMgr();
						lpChartMgr->RemoveDataColumn(strChartKey, strColumnName);
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to remove a data chart.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to remove a data chart.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void AddDataColumn(String *sChartKey, String *sColumnName, String *sModuleName, String *sType, String *sXml)
			{
				AnimatLibrary::Charting::DataColumn *lpColumn = NULL;

				try
				{
					if(m_lpSim) 
					{
						string strChartKey = Util::StringToStd(sChartKey);
						string strColumnName = Util::StringToStd(sColumnName);
						string strModuleName = Util::StringToStd(sModuleName);
						string strType = Util::StringToStd(sType);
						string strXml = Util::StringToStd(sXml);

						lpColumn = dynamic_cast<AnimatLibrary::Charting::DataColumn *>(m_lpSim->CreateObject(strModuleName, "DataColumn", strType));
						
						CStdXml oXml;
						oXml.Deserialize(strXml);
						oXml.FindElement("Data");
						oXml.FindChildElement("DataColumn");

						lpColumn->Load(m_lpSim, oXml);

						AnimatLibrary::Charting::DataChartMgr *lpChartMgr = m_lpSim->DataChartMgr();
						lpChartMgr->AddDataColumn(strChartKey, lpColumn);
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to add a data chart.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					if(lpColumn) delete lpColumn;
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to add a data chart.";
					if(lpColumn) delete lpColumn;
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void ModifyDataColumn(String *sChartKey, String *sColumnName, String *sDataType)
			{
				try
				{
					if(m_lpSim) 
					{
						string strChartKey = Util::StringToStd(sChartKey);
						string strColumnName = Util::StringToStd(sColumnName);
						string strDataType = Util::StringToStd(sDataType);

						AnimatLibrary::Charting::DataChartMgr *lpChartMgr = m_lpSim->DataChartMgr();
						lpChartMgr->ModifyDataColumn(strChartKey, strColumnName, strDataType);
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to modify a data chart.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to modify a data chart.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void SetDataColumnIndex(String *sChartKey, String *sColumnName, int iIndex)
			{
				try
				{
					if(m_lpSim) 
					{
						string strChartKey = Util::StringToStd(sChartKey);
						string strColumnName = Util::StringToStd(sColumnName);

						AnimatLibrary::Charting::DataChartMgr *lpChartMgr = m_lpSim->DataChartMgr();
						lpChartMgr->SetDataColumnIndex(strChartKey, strColumnName, iIndex);
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to set the index of a data column.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to set the index of a data column.";
					throw new System::Exception(m_strErrorMessage);
				}
			}


			System::Boolean FindStimulus(String *sKey, System::Boolean bThrowError)
			{
				try
				{
					if(m_lpSim) 
					{
						string strKey = Util::StringToStd(sKey);
						
						AnimatLibrary::ExternalStimuli::ExternalStimuliMgr *lpStimMgr = m_lpSim->ExternalStimuliMgr();
						if(lpStimMgr->Find(strKey, FALSE))
							return true;
						else
							return false;
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to find a stimulus.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to find a stimulus.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void RemoveStimulus(String *sKey)
			{
				try
				{
					if(m_lpSim) 
					{
						string strKey = Util::StringToStd(sKey);

						AnimatLibrary::ExternalStimuli::ExternalStimuliMgr *lpStimMgr = m_lpSim->ExternalStimuliMgr();
						lpStimMgr->Remove(m_lpSim, strKey);
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to remove a stimulus.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to remove a stimulus.";
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void AddStimulus(String *sStimulusKey, String *sStimulusName, String *sModuleName, String *sType, String *sXml)
			{
				AnimatLibrary::ExternalStimuli::ExternalStimulus *lpStimulus = NULL;

				try
				{
					if(m_lpSim) 
					{
						string strStimulusKey = Util::StringToStd(sStimulusKey);
						string strStimulusName = Util::StringToStd(sStimulusName);
						string strModuleName = Util::StringToStd(sModuleName);
						string strType = Util::StringToStd(sType);
						string strXml = Util::StringToStd(sXml);

						lpStimulus = dynamic_cast<AnimatLibrary::ExternalStimuli::ExternalStimulus *>(m_lpSim->CreateObject(strModuleName, "ExternalStimulus", strType));
						
						CStdXml oXml;
						oXml.Deserialize(strXml);
						oXml.FindElement("Stimuli");
						oXml.FindChildElement("Stimulus");

						lpStimulus->Load(m_lpSim, oXml);

						AnimatLibrary::ExternalStimuli::ExternalStimuliMgr *lpStimMgr = m_lpSim->ExternalStimuliMgr();
						lpStimMgr->Add(m_lpSim, lpStimulus);
					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to add a stimulus.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					if(lpStimulus) delete lpStimulus;
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to add a stimulus.";
					if(lpStimulus) delete lpStimulus;
					throw new System::Exception(m_strErrorMessage);
				}
			}

			void ModifyStimulus(String *sStimulusKey, String *sStimulusName, String *sModuleName, String *sType, System::Boolean bEnabled, String *sXml)
			{
				AnimatLibrary::ExternalStimuli::ExternalStimulus *lpStimulus = NULL;
				AnimatLibrary::ExternalStimuli::ExternalStimulus *lpOldStimulus = NULL;

				try
				{
					if(m_lpSim) 
					{
						string strStimulusKey = Util::StringToStd(sStimulusKey);
						string strStimulusName = Util::StringToStd(sStimulusName);
						string strModuleName = Util::StringToStd(sModuleName);
						string strType = Util::StringToStd(sType);
						string strXml = Util::StringToStd(sXml);

						AnimatLibrary::ExternalStimuli::ExternalStimuliMgr *lpStimMgr = m_lpSim->ExternalStimuliMgr();
						lpOldStimulus = dynamic_cast<AnimatLibrary::ExternalStimuli::ExternalStimulus *>(lpStimMgr->Find(strStimulusKey, FALSE));

						if(bEnabled)
						{
							lpStimulus = dynamic_cast<AnimatLibrary::ExternalStimuli::ExternalStimulus *>(m_lpSim->CreateObject(strModuleName, "ExternalStimulus", strType));
							
							CStdXml oXml;
							oXml.Deserialize(strXml);
							oXml.FindElement("Stimuli");
							oXml.FindChildElement("Stimulus");

							lpStimulus->Load(m_lpSim, oXml);

							if(lpOldStimulus)
							{
								lpStimulus->Modify(m_lpSim, lpOldStimulus);

								//Remove the old stimulus
								lpStimMgr->Remove(m_lpSim, strStimulusKey);
							}

							//Add the new stimulus
							lpStimMgr->Add(m_lpSim, lpStimulus);
						}
						else if(lpOldStimulus)
							lpStimMgr->Remove(m_lpSim, strStimulusKey);

					}
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to add a stimulus.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					if(lpStimulus) delete lpStimulus;
					throw new System::Exception(m_strErrorMessage);
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to add a stimulus.";
					if(lpStimulus) delete lpStimulus;
					throw new System::Exception(m_strErrorMessage);
				}
			}

		protected: 
			AnimatLibrary::Simulator *m_lpSim;
			bool m_bPaused;
			System::Boolean m_bIsLoaded;
			System::String *m_strProjectFile;
			System::Threading::Thread *m_newThread;
			IntPtr m_hParentWnd;
			System::String *m_strErrorMessage;
			AnimatTools::Interfaces::Logger *m_lpLogger;

			void RunSimulator()
			{
				int iHandle = 0;

				try
				{
					LogMsg(Logger::enumLogLevel::Info, "Starting RunSimulator");

					if(!m_lpSim)
						throw new System::Exception("You must first create a simulation before you can run it.");

					//m_lpSim = AnimatLibrary::Simulator::CreateSimulator(Util::StringToStd(m_strProjectFile));

					//System::Windows::Forms::MessageBox::Show(this, "Test", "test", System::Windows::Forms::MessageBoxButtons::YesNo,
					//								System::Windows::Forms::MessageBoxIcon::Question, System::Windows::Forms::MessageBoxDefaultButton::Button1, 
					//								System::Windows::Forms::MessageBoxOptions::RightAlign);

//#pragma push_macro("MessageBox")
//#undef MessageBox     
//System::Windows::Forms::MessageBox::Show("About to Initialize.");     
//#pragma pop_macro("MessageBox")

					LogMsg(Logger::enumLogLevel::Debug, "About to initialize the simulator");

					void *hWnd = (void *) m_hParentWnd.ToInt32();
					m_lpSim->ParentHwnd(hWnd);
					//m_lpSim->Load();
					m_lpSim->Paused(m_bPaused);
					m_lpSim->Initialize(0, NULL);

					LogMsg(Logger::enumLogLevel::Debug, "Finished initializing the simulator");

//#pragma push_macro("MessageBox")
//#undef MessageBox     
//System::Windows::Forms::MessageBox::Show("Finished Initializing.");     
//#pragma pop_macro("MessageBox")

					//We need to get the handle for this instance of the simulation wrapper and pass it
					//to the simulator object so it can pass this back in callbacks. We will cast this
					//into a gc pointer for our wrapper and then fire the appropriate events to communicate
					//information about the simulator back up to the forms.
					iHandle = (GCHandle::op_Explicit(GCHandle::Alloc(this))).ToInt32();

					m_lpSim->ManagedInstance( (void *) iHandle);
					m_lpSim->UpdateDataCallback( (ManagedCallbackPtr) UpdateDataCallback);
					m_lpSim->StartSimulationCallback( (ManagedCallbackPtr) StartSimulationCallback);
					m_lpSim->PauseSimulationCallback( (ManagedCallbackPtr) PauseSimulationCallback);
					m_lpSim->EndingSimulationCallback( (ManagedCallbackPtr) EndingSimulationCallback);

//#pragma push_macro("MessageBox")
//#undef MessageBox     
//System::Windows::Forms::MessageBox::Show("Set Callbacks.");     
//#pragma pop_macro("MessageBox")

					LogMsg(Logger::enumLogLevel::Debug, "About to start simulation processing loop");

					m_lpSim->Simulate();

					LogMsg(Logger::enumLogLevel::Debug, "Finished simulation processing loop");

					//#pragma push_macro("MessageBox")
//#undef MessageBox     
//System::Windows::Forms::MessageBox::Show("Simulated.");     
//#pragma pop_macro("MessageBox")

					if(iHandle) GCHandle::op_Explicit(iHandle).Free();
					if(m_lpSim) delete m_lpSim;
					m_lpSim = NULL;

					LogMsg(Logger::enumLogLevel::Info, "Finished RunSimulator");
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while attempting to start the simulation.\nError: " + oError.m_strError;
					m_strErrorMessage = strError.c_str();
					FireSimulationErrorEvent();
					if(iHandle) GCHandle::op_Explicit(iHandle).Free();
					if(m_lpSim) delete m_lpSim;
					m_lpSim = NULL;
				}
				catch(...)
				{
					m_strErrorMessage = "An unknown error occurred while attempting to start the simulation.";
					FireSimulationErrorEvent();
					if(iHandle) GCHandle::op_Explicit(iHandle).Free();
					if(m_lpSim) delete m_lpSim;
					m_lpSim = NULL;
				}
			}

			void LogMsg(AnimatTools::Interfaces::Logger::enumLogLevel eLevel, System::String *sMessage)
			{
				if(m_lpLogger)
					m_lpLogger->LogMsg(eLevel, sMessage);
			}

			void Dispose(Boolean disposing)
			{
				if (disposing && components)
				{
					components->Dispose();
				}
				__super::Dispose(disposing);
			}
					
		private:
			/// <summary>
			/// Required designer variable.
			/// </summary>
			System::ComponentModel::Container *components;
			
			/// <summary>
			/// Required method for Designer support - do not modify
			/// the contents of this method with the code editor.
			/// </summary>		
			void InitializeComponent(void)
			{
				components = new System::ComponentModel::Container();
			}
		};


		//********************* Stand Alone Callback Functions *******************************
		//These functions are the callbacks that get executed by the CAlSimulator object. For example, the 
		//UpdateDataCallback is called whenever the simulator has collected enough data. When it calls this
		//method it passes back the handle to an instance of a managed simulator object as a void pointer.
		//We then cast that void to a specific instance of a simulator and then fire the update events.

		void UpdateDataCallback(void *lpSim)
		{
			SimulatorInterface *lpSimulator = __try_cast<SimulatorInterface*>((GCHandle::op_Explicit(lpSim)).Target);
			if(lpSimulator)
				lpSimulator->FireUpdateDataEvent();
		}

		void StartSimulationCallback(void *lpSim)
		{
			SimulatorInterface *lpSimulator = __try_cast<SimulatorInterface*>((GCHandle::op_Explicit(lpSim)).Target);
			if(lpSimulator)
				lpSimulator->FireStartSimulationEvent();
		}

		void PauseSimulationCallback(void *lpSim)
		{
			SimulatorInterface *lpSimulator = __try_cast<SimulatorInterface*>((GCHandle::op_Explicit(lpSim)).Target);
			if(lpSimulator)
				lpSimulator->FirePauseSimulationEvent();
		}

		void EndingSimulationCallback(void *lpSim)
		{
			SimulatorInterface *lpSimulator = __try_cast<SimulatorInterface*>((GCHandle::op_Explicit(lpSim)).Target);
			if(lpSimulator)
				lpSimulator->FireEndingSimulationEvent();
		}

		void ResetSimulationCallback(void *lpSim)
		{
			SimulatorInterface *lpSimulator = __try_cast<SimulatorInterface*>((GCHandle::op_Explicit(lpSim)).Target);
			if(lpSimulator)
				lpSimulator->FireResetSimulationEvent();
		}

	}
}