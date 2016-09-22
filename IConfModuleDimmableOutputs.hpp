#pragma once


class IConfModuleDimmableOutputs
{
	public:
		// Empty virtual destructor for proper cleanup
		virtual ~IConfModuleDimmableOutputs() {}

		virtual bool setDimmableOutput(int position, uint8_t value) = 0;
		virtual uint8_t getDimmableOutput(int position) = 0;
};
