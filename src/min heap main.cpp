#include "MinHeap.h"
#include "PatientRecordsBST.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <iostream>

MinHeap emergencyQueue;
PatientRecordsBST patientBST;

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hospital Emergency System", NULL, NULL);
    glfwMakeContextCurrent(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 120");

    static int id = 0, age = 0, priority = 3;
    static char name[50], symptoms[100];

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Patient Registration");

        ImGui::InputInt("ID", &id);
        ImGui::InputText("Name", name, 50);
        ImGui::InputInt("Age", &age);
        ImGui::InputText("Symptoms", symptoms, 100);
        ImGui::InputInt("Priority (1-3)", &priority);

        if (ImGui::Button("Register Patient")) {
            Patient p{ id, name, age, symptoms, priority };
            emergencyQueue.insert(p);

            PatientData pd{ id, name, age, symptoms, priority };
            patientBST.insertPatient(pd);
        }

        ImGui::Separator();
        ImGui::Text("Emergency Queue");

        for (const auto& p : emergencyQueue.getPatients()) {
            ImGui::Text("ID %d | %s | Priority %d", p.id, p.name.c_str(), p.priority);
        }

        if (ImGui::Button("Treat Next Patient")) {
            emergencyQueue.extractMin();
        }

        ImGui::End();

        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
