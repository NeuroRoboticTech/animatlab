using System;

namespace Crownwood.Magic.Network
{
	public enum PingResponseType
	{
		Ok = 0,
		CouldNotResolveHost,
		RequestTimedOut,
		ConnectionError,
		InternalError,
		Canceled
	}
}
