#pragma once


class Renderable {
protected:
	GLuint VAO;
public:
	virtual void draw(bool drawTriangles) const = 0;
	virtual ~Renderable() = default;
};

