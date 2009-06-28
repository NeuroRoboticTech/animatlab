#pragma once

#include "StdAfx.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace AnimatTools;

namespace AnimatTools
{
	namespace Forms
	{
		/// <summary> 
		/// Summary for SimulatorView
		///
		/// WARNING: If you change the name of this class, you will need to change the 
		///          'Resource File Name' property for the managed resource compiler tool 
		///          associated with all .resx files this class depends on.  Otherwise,
		///          the designers will not be able to interact properly with localized
		///          resources associated with this form.
		/// </summary>
		//AnimatTools::Forms::AnimatForm
	public __gc class SimulatorView : public AnimatTools::Forms::AnimatForm
		{
		public: 
			SimulatorView(void)
			{
				InitializeComponent();
				m_lpSim = NULL;
				Simulate();
			}

			/*
			virtual void LoadData(ManagedStdUtils::StdXml &oXml)
			{
			}
			*/

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
			System::ComponentModel::Container* components;

			/// <summary>
			/// Required method for Designer support - do not modify
			/// the contents of this method with the code editor.
			/// </summary>
			void InitializeComponent(void)
			{ 
				this->components = new System::ComponentModel::Container();
				this->Size = System::Drawing::Size(300,300);
				this->Text = "SimulatorView";
			}		

		protected:
			CAlSimulator *m_lpSim;
			System::Threading::Thread *m_lpThread;

			void Simulate()
			{
				System::Threading::ThreadStart *startThread = new System::Threading::ThreadStart(this, &SimulatorView::RunSimulator);
				m_lpThread = new System::Threading::Thread(startThread);
				m_lpThread->Start();
			}

			void RunSimulator()
			{
			try
			{
				m_lpSim = CAlSimulator::CreateSimulator("C:\\Projects\\bin\\Experiments\\Robot\\Robot.asim");
				void *hWnd = (void *) this->Handle.ToInt32();

				m_lpSim->Load();
				m_lpSim->ParentHwnd(hWnd);
				//m_lpSim->Initialize(0, NULL);
				m_lpSim->RunSimulation();

				if(m_lpSim) delete m_lpSim;
			}
			catch(CStdErrorInfo oError)
			{
				if(m_lpSim) delete m_lpSim;
				//printf("Error occurred %s\n", oError.m_strError) ;
			}
			catch(...)
			{
				if(m_lpSim) delete m_lpSim;
				//printf("An Unknown Error occurred.\n") ;
			}
			}


		};
	}
}
