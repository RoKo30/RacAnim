#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>


class MyGLM  {
public:
	static glm::mat4 translate3D(glm::vec3 translateVec) {
		return glm::mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, translateVec.x, translateVec.y, translateVec.z, 1.0);
	}

    static glm::mat4 scale3D(glm::vec3 scaleVec) {
		return glm::mat4(scaleVec.x, 0.0, 0.0, 0.0, 0.0, scaleVec.y, 0.0, 0.0, 0.0, 0.0, scaleVec.z, 0.0, 0.0, 0.0, 0.0, 1.0);
	}

    static glm::mat4 rotate3D(glm::vec3 axis, float angle) {
		axis = glm::normalize(axis);
		angle = glm::radians(angle);
		return glm::mat4(axis.x * axis.x + (1 - axis.x * axis.x) * glm::cos(angle), axis.x * axis.y * (1 - glm::cos(angle)) + axis.z * glm::sin(angle), axis.x * axis.z * (1 - glm::cos(angle)) - axis.y * glm::sin(angle), 0,
						 axis.x * axis.y * (1 - glm::cos(angle)) - axis.z * glm::sin(angle), axis.y * axis.y + (1 - axis.y * axis.y) * glm::cos(angle), axis.z * axis.y * (1 - glm::cos(angle)) + axis.x * glm::sin(angle), 0,
						 axis.x * axis.z * (1 - glm::cos(angle)) + axis.y * glm::sin(angle), axis.z * axis.y * (1 - glm::cos(angle)) - axis.x * glm::sin(angle), axis.z * axis.z + (1 - axis.z * axis.z) * glm::cos(angle), 0,
						 0                                                                 , 0                                                                 , 0                                                        , 1);
	}

    static glm::mat4 lookAtMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 viewUp) {
        // z axis
        glm::vec3 forward = glm::normalize(eye - center);

        // x axis
        glm::vec3 right = glm::normalize(glm::cross(glm::normalize(viewUp), forward));

        // y axis
        glm::vec3 up = glm::cross(forward, right);

        /*glm::mat4 view(1.0f);

        view[0][0] = right.x;
        view[1][0] = right.y;
        view[2][0] = right.z;

        view[0][1] = up.x;
        view[1][1] = up.y;
        view[2][1] = up.z;

        view[0][2] = forward.x;
        view[1][2] = forward.y;
        view[2][2] = forward.z;

        view[3][0] = -glm::dot(right, eye);
        view[3][1] = -glm::dot(up, eye);
        view[3][2] = -glm::dot(forward, eye);*/

        glm::mat4 view = glm::mat4(
            right.x, up.x, forward.x, 0.0f,
            right.y, up.y, forward.y, 0.0f,
            right.z, up.z, forward.z, 0.0f,
            -glm::dot(right, eye), -glm::dot(up, eye), -glm::dot(forward, eye), 1.0f
        );

        return view;
	}
	
    static glm::mat4 frustum(float l, float r, float b, float t, float n, float f) {
        glm::mat4 matrix(0.0f);  

        matrix[0][0] = (2.0f * n) / (r - l);                 
        matrix[1][1] = (2.0f * n) / (t - b);                 
        matrix[2][0] = (r + l) / (r - l);                    
        matrix[2][1] = (t + b) / (t - b);                    
        matrix[2][2] = -(f + n) / (f - n);                   
        matrix[2][3] = -(2.0f * f * n) / (f - n);            
        matrix[3][2] = -1.0f;                                
        matrix[3][3] = 0.0f;                                 

        return glm::transpose(matrix);
    }

};

