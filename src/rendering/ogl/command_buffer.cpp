#include "command_buffer.h"
#include "command_buffer.h"
#include "command_buffer.h"
#include "command_buffer.h"

command_buffer::command_buffer() :
	buf(buffer_access_type::DynamicDraw, buffer_type::Command)
{

}

void command_buffer::push(indirect_draw_command&& cmd)
{
	cmds.push_back(std::move(cmd));
}

void command_buffer::submit()
{
	buf.setData(cmds.data(), sizeof(indirect_draw_command) * cmds.size());
	cmds.clear();
}

void command_buffer::bind()
{
	buf.bind();
}
