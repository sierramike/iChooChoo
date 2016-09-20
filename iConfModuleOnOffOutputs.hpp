#pragma once

class iConfModuleOnOffOutputs
{
	public:
		// Empty virtual destructor for proper cleanup
		virtual ~iConfModuleOnOffOutputs() {}

		virtual void setOutput(int position, bool value) = 0;
		virtual bool getOutput(int position) = 0;
};