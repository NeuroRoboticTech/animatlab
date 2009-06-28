#pragma once

namespace AnimatTools
{
	namespace Interfaces
	{
		class Util
		{
		public:
			Util(void);
			virtual ~Util(void);

			static std::string StringToStd( System::String *s);
		};
	}
}

