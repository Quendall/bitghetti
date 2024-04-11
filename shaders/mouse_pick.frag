#version 330 core

uniform uint entity_id;
out uint frag_id;

void main()
{
	frag_id = entity_id;
}