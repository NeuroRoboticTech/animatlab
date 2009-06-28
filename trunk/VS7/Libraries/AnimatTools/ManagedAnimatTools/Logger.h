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

		/// <summary> 
		/// Summary for Logger
		/// </summary>
		__gc public class Logger :  public System::ComponentModel::Component
		{
		public:
			Logger(void)
			{
				InitializeComponent();
			}
			Logger(System::ComponentModel::IContainer *container) : components(0)
			{
				/// <summary>
				/// Required for Windows.Forms Class Composition Designer support
				/// </summary>

				container->Add(this);
				InitializeComponent();
			}
 
			__value enum enumLogLevel
				{
					None = 0, // no trace
					Error = 10, // only trace error
					Info = 20, // some extra info
					Debug = 30, // debugging info
					Detail = 40 // detailed debugging info
				};

			__property void set_LogPrefix(System::String *sLogFile)
			{
				try
				{
					string strLogFile = Util::StringToStd(sLogFile);
					Std_SetLogFilePrefix(strLogFile);
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while SetLogPrefix.\nError: " + oError.m_strError;
					throw new System::Exception(strError.c_str());
				}
				catch(...)
				{throw new System::Exception("An unknown error occurred while attempting to SetLogPrefix.");}
			}

			__property System::String *get_LogPrefix()
			{
				try
				{
					string strPrefix = Std_GetLogFilePrefix();
					 System::String *strVal = strPrefix.c_str();
					 return strVal;
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while get_LogPrefix.\nError: " + oError.m_strError;
					throw new System::Exception(strError.c_str());
				}
				catch(...)
				{throw new System::Exception("An unknown error occurred while attempting to get_LogPrefix.");}
			}

			__property void set_TraceLevel(enumLogLevel eLevel)
			{
				try
				{
					Std_SetTraceLevel(eLevel);
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while TraceLevel.\nError: " + oError.m_strError;
					throw new System::Exception(strError.c_str());
				}
				catch(...)
				{throw new System::Exception("An unknown error occurred while attempting to TraceLevel.");}
			}

			__property enumLogLevel get_TraceLevel()
			{
				try
				{
					 return (enumLogLevel) Std_GetTraceLevel();
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while Std_SetTraceLevel.\nError: " + oError.m_strError;
					throw new System::Exception(strError.c_str());
				}
				catch(...)
				{throw new System::Exception("An unknown error occurred while attempting to Std_SetTraceLevel."); return enumLogLevel::None;}
			}

			void LogMsg(enumLogLevel eLevel, System::String *sMessage)
			{
				try
				{
					string strMessage = Util::StringToStd(sMessage);
					Std_LogMsg(eLevel, strMessage, "", -1, FALSE);
				}
				catch(CStdErrorInfo oError)
				{
					string strError = "An error occurred while LogMsg.\nError: " + oError.m_strError;
					throw new System::Exception(strError.c_str());
				}
				catch(...)
				{throw new System::Exception("An unknown error occurred while attempting to LogMsg.");}

			}

		protected: 
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

	}
}