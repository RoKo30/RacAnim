#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class BezieBuilder {


	static unsigned long long factorial(int n) {
		unsigned long long res = 1;
		for (int i = 2; i <= n; ++i) res *= i;
		return res;
	}

	static unsigned long long binomialCoeff(int n, int i) {
		return factorial(n) / (factorial(i) * factorial(n - i));
	}

	static float bernstein(int i, int n, float t) {
		float coeff = static_cast<float>(binomialCoeff(n, i));
		return coeff * std::pow(t, i) * std::pow(1 - t, n - i);
	}

public:
	// Bezie
	static std::vector<glm::vec3> bezierBernstein(const std::vector<glm::vec3>& controlPoints, int resolution = 100) {
		std::vector<glm::vec3> curvePoints;
		int n = static_cast<int>(controlPoints.size()) - 1;

		for (int step = 0; step <= resolution; ++step) {
			float t = step / static_cast<float>(resolution);
			glm::vec3 point(0.0f);

			for (int i = 0; i <= n; ++i) {
				float coeff = binomialCoeff(n, i) * std::pow(t, i) * std::pow(1 - t, n - i);
				point += coeff * controlPoints[i];
			}

			curvePoints.push_back(point);
		}

		return curvePoints;
	}


	static std::vector<glm::vec3> bezierInterpolated(const std::vector<glm::vec3>& interpolationPoints, int resolution = 100) {
		int n = interpolationPoints.size() - 1;

		glm::mat4 F(0.0f);
		std::vector<float> ts = { 0.0f, 1.0f / n, 2.0f / n, 1.0f };

		for (int row = 0; row <= n; ++row) {
			for (int col = 0; col <= n; ++col) {
				F[row][col] = bernstein(col, n, ts[row]);
			}
		}

		glm::mat4 F_inv = glm::inverse(F);

		std::vector<glm::vec3> controlPoints(4, glm::vec3(0.0f));

		for (int row = 0; row <= n; ++row) {
			glm::vec3 sum(0.0f);
			for (int col = 0; col <= n; ++col) {
				sum += F_inv[row][col] * interpolationPoints[col];
			}
			controlPoints[row] = sum;
		}

		return bezierBernstein(controlPoints, resolution);
	}


};