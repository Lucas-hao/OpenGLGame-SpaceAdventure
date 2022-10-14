#pragma once

class ColorTexture 
{
public:
	void setupTexture(const char* texturePath);
	void bind(unsigned int slot) const;
	void unbind() const;

private:
	unsigned int ID = 0;
	unsigned int Width = 0, Height = 0, BPP = 0;
};
