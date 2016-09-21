#pragma once

class IConfModuleOnOffOutputs
{
	public:
		// Empty virtual destructor for proper cleanup
		virtual ~IConfModuleOnOffOutputs() {}

		virtual void setOutput(int position, bool value) = 0;
		virtual bool getOutput(int position) = 0;
};
