#pragma once

class Command
{
public:
	virtual ~Command() {}
	virtual void execute() = 0;

	virtual void undo() = 0;
	virtual void redo() = 0;
};