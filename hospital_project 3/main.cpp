#include "PatientBackend.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <ctime>
#include <cstring>
#include <cstdio>

class GUIManager {
private:
    BackendInterface& backend;
    enum Screen { DASHBOARD, REGISTER, QUEUE, SEARCH };
    Screen currentScreen = DASHBOARD;
    
    char nameInput[128] = "";
    char ageInput[16] = "";
    char symptomsInput[256] = "";
    int selectedPriority = 2;
    char statusMessage[256] = "";
    bool showStatus = false;
    
    char searchIDInput[16] = "";
    Patient* searchResult = nullptr;
    bool searchPerformed = false;
    
public:
    GUIManager(BackendInterface& be) : backend(be) {}
    
    void render() {
        renderMenuBar();
        
        ImGui::Begin("Hospital Emergency Management System", nullptr, 
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        ImGui::SetWindowSize(ImVec2(1000, 700), ImGuiCond_FirstUseEver);
        
        switch (currentScreen) {
            case DASHBOARD: renderDashboard(); break;
            case REGISTER: renderRegistration(); break;
            case QUEUE: renderQueue(); break;
            case SEARCH: renderSearch(); break;
        }
        
        ImGui::End();
    }
    
private:
    void renderMenuBar() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::MenuItem("Dashboard", nullptr, currentScreen == DASHBOARD)) {
                currentScreen = DASHBOARD;
            }
            if (ImGui::MenuItem("Register Patient", nullptr, currentScreen == REGISTER)) {
                currentScreen = REGISTER;
            }
            if (ImGui::MenuItem("View Queue", nullptr, currentScreen == QUEUE)) {
                currentScreen = QUEUE;
            }
            if (ImGui::MenuItem("Search Patient", nullptr, currentScreen == SEARCH)) {
                currentScreen = SEARCH;
            }
            ImGui::EndMainMenuBar();
        }
    }
    
    void renderDashboard() {
        ImGui::Text("Emergency Department Overview");
        ImGui::Separator();
        ImGui::Spacing();
        
        int total = backend.getTotalPatients();
        int critical = backend.getPatientsByPriority(1);
        int urgent = backend.getPatientsByPriority(2);
        int standard = backend.getPatientsByPriority(3);
        
        ImGui::BeginChild("Stats", ImVec2(0, 200), true);
        ImGui::Text("Total Patients Waiting: %d", total);
        ImGui::Spacing();
        
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
        ImGui::Text("Critical (Priority 1): %d", critical);
        ImGui::PopStyleColor();
        
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.0f, 1.0f));
        ImGui::Text("Urgent (Priority 2): %d", urgent);
        ImGui::PopStyleColor();
        
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
        ImGui::Text("Standard (Priority 3): %d", standard);
        ImGui::PopStyleColor();
        
        ImGui::EndChild();
        ImGui::Spacing();
        
        ImGui::BeginChild("NextPatient", ImVec2(0, 150), true);
        ImGui::Text("Next Patient to be Treated:");
        ImGui::Separator();
        
        auto queue = backend.getQueuedPatients();
        if (!queue.empty()) {
            Patient& next = queue[0];
            ImGui::Text("ID: %d", next.id);
            ImGui::Text("Name: %s", next.name.c_str());
            ImGui::Text("Priority: %s", next.priorityLabel.c_str());
        } else {
            ImGui::Text("No patients in queue");
        }
        ImGui::EndChild();
        
        ImGui::Spacing();
        if (ImGui::Button("Register New Patient", ImVec2(200, 40))) {
            currentScreen = REGISTER;
        }
        ImGui::SameLine();
        if (ImGui::Button("View Full Queue", ImVec2(200, 40))) {
            currentScreen = QUEUE;
        }
    }
    
    void renderRegistration() {
        ImGui::Text("Patient Registration");
        ImGui::Separator();
        ImGui::Spacing();
        
        ImGui::BeginChild("RegistrationForm", ImVec2(600, 500), true);
        
        ImGui::Text("Patient Name:");
        ImGui::InputText("##name", nameInput, 128);
        ImGui::Spacing();
        
        ImGui::Text("Age:");
        ImGui::InputText("##age", ageInput, 16, ImGuiInputTextFlags_CharsDecimal);
        ImGui::Spacing();
        
        ImGui::Text("Symptoms/Reason for Visit:");
        ImGui::InputTextMultiline("##symptoms", symptomsInput, 256, ImVec2(550, 100));
        ImGui::Spacing();
        
        ImGui::Text("Priority Level:");
        ImGui::RadioButton("Critical (Heart attack, Severe trauma)", &selectedPriority, 1);
        ImGui::RadioButton("Urgent (High fever, Fractures)", &selectedPriority, 2);
        ImGui::RadioButton("Standard (Minor injuries, Routine checkup)", &selectedPriority, 3);
        ImGui::Spacing();
        
        if (ImGui::Button("Submit Registration", ImVec2(200, 40))) {
            if (validateRegistrationForm()) registerPatient();
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear Form", ImVec2(150, 40))) clearRegistrationForm();
        
        if (showStatus) {
            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
            ImGui::Text("%s", statusMessage);
            ImGui::PopStyleColor();
        }
        
        ImGui::EndChild();
    }
    
    void renderQueue() {
        ImGui::Text("Patient Queue (Ordered by Priority)");
        ImGui::Separator();
        ImGui::Spacing();
        
        auto queue = backend.getQueuedPatients();
        if (queue.empty()) {
            ImGui::Text("No patients currently in queue");
            return;
        }
        
        if (ImGui::BeginTable("QueueTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("ID");
            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Age");
            ImGui::TableSetupColumn("Priority");
            ImGui::TableSetupColumn("Symptoms");
            ImGui::TableHeadersRow();
            
            for (const auto& patient : queue) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn(); ImGui::Text("%d", patient.id);
                ImGui::TableNextColumn(); ImGui::Text("%s", patient.name.c_str());
                ImGui::TableNextColumn(); ImGui::Text("%d", patient.age);
                ImGui::TableNextColumn();
                if (patient.priority == 1)
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
                else if (patient.priority == 2)
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.0f, 1.0f));
                else
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
                ImGui::Text("%s", patient.priorityLabel.c_str());
                ImGui::PopStyleColor();
                ImGui::TableNextColumn(); ImGui::Text("%s", patient.symptoms.c_str());
            }
            
            ImGui::EndTable();
        }
    }
    
    void renderSearch() {
        ImGui::Text("Search Patient by ID");
        ImGui::Separator();
        ImGui::Spacing();
        
        ImGui::BeginChild("SearchArea", ImVec2(600, 500), true);
        
        ImGui::Text("Enter Patient ID:");
        ImGui::InputText("##searchID", searchIDInput, 16, ImGuiInputTextFlags_CharsDecimal);
        ImGui::Spacing();
        
        if (ImGui::Button("Search", ImVec2(150, 40))) performSearch();
        ImGui::SameLine();
        if (ImGui::Button("Clear", ImVec2(150, 40))) {
            searchIDInput[0] = '\0';
            searchResult = nullptr;
            searchPerformed = false;
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        if (searchPerformed) {
            if (searchResult != nullptr) {
                ImGui::Text("Patient Found:");
                ImGui::Spacing();
                ImGui::Text("ID: %d", searchResult->id);
                ImGui::Text("Name: %s", searchResult->name.c_str());
                ImGui::Text("Age: %d", searchResult->age);
                ImGui::Text("Priority: %s", searchResult->priorityLabel.c_str());
                ImGui::Text("Symptoms: %s", searchResult->symptoms.c_str());
            } else {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
                ImGui::Text("Patient not found!");
                ImGui::PopStyleColor();
            }
        }
        
        ImGui::EndChild();
    }
    
    bool validateRegistrationForm() {
        if (strlen(nameInput) == 0) { strcpy(statusMessage, "Error: Name cannot be empty!"); showStatus = true; return false; }
        if (strlen(ageInput) == 0 || atoi(ageInput) <= 0) { strcpy(statusMessage, "Error: Please enter a valid age!"); showStatus = true; return false; }
        if (strlen(symptomsInput) == 0) { strcpy(statusMessage, "Error: Symptoms cannot be empty!"); showStatus = true; return false; }
        return true;
    }
    
    void registerPatient() {
        Patient newPatient;
        newPatient.id = backend.nextPatientID++;
        newPatient.name = std::string(nameInput);
        newPatient.age = atoi(ageInput);
        newPatient.symptoms = std::string(symptomsInput);
        newPatient.priority = selectedPriority;
        newPatient.priorityLabel = (selectedPriority == 1) ? "Critical" : (selectedPriority == 2) ? "Urgent" : "Standard";
        
        backend.addPatient(newPatient);
        sprintf(statusMessage, "Patient registered successfully! ID: %d", newPatient.id);
        showStatus = true;
        clearRegistrationForm();
    }
    
    void clearRegistrationForm() {
        nameInput[0] = '\0';
        ageInput[0] = '\0';
        symptomsInput[0] = '\0';
        selectedPriority = 2;
    }
    
    void performSearch() {
        if (strlen(searchIDInput) == 0) { searchResult = nullptr; searchPerformed = false; return; }
        int searchID = atoi(searchIDInput);
        searchResult = backend.searchPatient(searchID);
        searchPerformed = true;
    }
}; // End of GUIManager

int main() {
    if (!glfwInit()) return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(1200, 800, "Hospital ER Management", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    // <-- Store data in CSV
    BackendInterface backend("patients.csv");
    GUIManager gui(backend);
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        gui.render();
        
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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