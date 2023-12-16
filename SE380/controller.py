import numpy as np


class Controller:

    def __init__(self):
        self.y = np.zeros((2, 1))
        self.delta_t = 0.001

        self.A_f = np.array([[0, 1, 0], [0, 0, 1], [-1000, -300, -30]])
        self.B_f = np.array([[0], [0], [1]])
        self.C_f = np.array([[1000, 0, 0]])
        self.D_f = np.array([0])

        self.x_f1 = np.zeros((3, 1))
        self.x_f2 = np.zeros((3, 1))

        self.K_1 = self.K_2 = np.array([2.235, 0.51])

    def calculate_acceleration(self, y):
        y_dot = (y - self.y) / self.delta_t
        self.y = y
        y_dot_1 = y_dot[0]
        y_dot_2 = y_dot[1]

        self.x_f1 = self.x_f1 + self.delta_t * (self.A_f @ self.x_f1 +
                                                self.B_f * y_dot_1)
        self.x_f2 = self.x_f2 + self.delta_t * (self.A_f @ self.x_f2 +
                                                self.B_f * y_dot_2)

        y_dot_1_filtered = self.C_f @ self.x_f1 + self.D_f * y_dot_1
        y_dot_2_filtered = self.C_f @ self.x_f2 + self.D_f * y_dot_2

        x_1 = np.array([y[0], y_dot_1_filtered])
        x_2 = np.array([y[1], y_dot_2_filtered])

        u_1 = -np.dot(self.K_1, x_1)
        u_2 = -np.dot(self.K_2, x_2)

        return np.array([u_1, u_2]).reshape(2, 1)
