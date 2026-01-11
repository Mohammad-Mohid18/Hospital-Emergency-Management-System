#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <ctime>
#include <cstring>
#include <cstdio>
#include "MinHeap.h"
#include "PatientRecordsBST.h"

// Backend Integration Class
class BackendInterface {
private:
    MinHeap priorityQueue;
    PatientRecordsBST patientRecords;
    int nextPatientID = 1001;
    
public:
BackendInterface() {
    // Try to load existing data
    patientRecords.loadFromFile("patients.csv");
    
    // Set next ID based on loaded data
    auto records = patientRecords.getAllPatients();
    if (!records.empty()) {
        int maxID = 1000;
        for (const auto& p : records) {
            if (p.patientID > maxID) maxID = p.patientID;
        }
        nextPatientID = maxID + 1;
    }
}
    
    ~BackendInterface() {
        // Auto-save on exit
        patientRecords.saveToFile("patients.csv");
    }
    
    void addPatient(const Patient& p) {
        // Add to priority queue (MinHeap)
        priorityQueue.insert(p);
        
        // Add to BST for searching
        PatientData pd;
        pd.patientID = p.id;
        pd.name = p.name;
        pd.age = p.age;
        pd.symptoms = p.symptoms;
        pd.priorityLevel = p.priority;
        patientRecords.insertPatient(pd);
    }
    
    std::vector<Patient> getQueuedPatients() {
        return priorityQueue.getPatients();
    }
    
    Patient* searchPatient(int id) {
        PatientData* pd = patientRecords.searchPatient(id);
        if (pd != nullptr) {
            static Patient p;
            p.id = pd->patientID;
            p.name = pd->name;
            p.age = pd->age;
            p.symptoms = pd->symptoms;
            p.priority = pd->priorityLevel;
            return &p;
        }
        return nullptr;
    }
    
    int getTotalPatients() {
        return priorityQueue.getPatients().size();
    }
    
    int getPatientsByPriority(int priority) {
        int count = 0;
        for (const auto& p : priorityQueue.getPatients()) {
            if (p.priority == priority) count++;
        }
        return count;
    }
    
    int getNextID() {
        return nextPatientID++;
    }
    
    Patient getNextPatient() {
        return priorityQueue.peek();
    }
    
    void treatNextPatient() {
        priorityQueue.extractMin();
    }
    
    void saveToFile() {
        patientRecords.saveToFile("patients.csv");
    }
    
    std::vector<PatientData> getAllRecords() {
        return patientRecords.getAllPatients();
    }
};

// GUI Manager Class
class GUIManager {
private:
    BackendInterface& backend;
    enum Screen { DASHBOARD, REGISTER, QUEUE, SEARCH, RECORDS };
    Screen currentScreen = DASHBOARD;
    
    char nameInput[128] = "";
    char ageInput[16] = "";
    char symptomsInput[256] = "";
    int selectedPriority = 2;
    char statusMessage[256] = "";
    bool showStatus = false;
    float statusTimer = 0.0f;
    
    char searchIDInput[16] = "";
    Patient* searchResult = nullptr;
    bool searchPerformed = false;
    
    // Larger fonts
    ImFont* headerFont = nullptr;
    ImFont* normalFont = nullptr;
    
public:
    GUIManager(BackendInterface& be) : backend(be) {}
    
    void render() {
        renderMenuBar();
        
        // Fancy main window with larger size
        ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(1200, 780), ImGuiCond_Always);
        
        ImGui::Begin("Hospital Emergency Management System", nullptr, 
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        
        // Status message timer
        if (showStatus) {
            statusTimer += ImGui::GetIO().DeltaTime;
            if (statusTimer > 3.0f) {
                showStatus = false;
                statusTimer = 0.0f;
            }
        }
        
        switch (currentScreen) {
            case DASHBOARD: renderDashboard(); break;
            case REGISTER: renderRegistration(); break;
            case QUEUE: renderQueue(); break;
            case SEARCH: renderSearch(); break;
            case RECORDS: renderAllRecords(); break;
        }
        
        ImGui::End();
    }
    
private:
    void renderMenuBar() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::MenuItem("🏠 Dashboard", nullptr, currentScreen == DASHBOARD)) {
                currentScreen = DASHBOARD;
            }
            if (ImGui::MenuItem("➕ Register Patient", nullptr, currentScreen == REGISTER)) {
                currentScreen = REGISTER;
            }
            if (ImGui::MenuItem("📋 View Queue", nullptr, currentScreen == QUEUE)) {
                currentScreen = QUEUE;
            }
            if (ImGui::MenuItem("🔍 Search Patient", nullptr, currentScreen == SEARCH)) {
                currentScreen = SEARCH;
            }
            if (ImGui::MenuItem("📁 All Records", nullptr, currentScreen == RECORDS)) {
                currentScreen = RECORDS;
            }
            
            ImGui::Separator();
            
            if (ImGui::MenuItem("💾 Save Data")) {
                backend.saveToFile();
                strcpy(statusMessage, "✓ Data saved successfully!");
                showStatus = true;
                statusTimer = 0.0f;
            }
            
            ImGui::EndMainMenuBar();
        }
    }
    
    void renderDashboard() {
        ImGui::PushFont(ImGui::GetFont());
        
        // Title with larger font
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.8f, 1.0f, 1.0f));
        ImGui::SetWindowFontScale(1.8f);
        ImGui::Text("🏥 Emergency Department Overview");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PopStyleColor();
        
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Spacing();
        
        int total = backend.getTotalPatients();
        int critical = backend.getPatientsByPriority(1);
        int urgent = backend.getPatientsByPriority(2);
        int standard = backend.getPatientsByPriority(3);
        
        // Statistics cards with fancy styling
        ImGui::BeginChild("Stats", ImVec2(0, 250), true, ImGuiWindowFlags_NoScrollbar);
        
        ImGui::SetWindowFontScale(1.4f);
        ImGui::Text("📊 Patient Statistics");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        ImGui::SetWindowFontScale(1.3f);
        ImGui::Text("Total Patients Waiting: %d", total);
        ImGui::Spacing();
        ImGui::Spacing();
        
        // Critical (Red with background)
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
        ImGui::Text("🔴 Critical (Priority 1): %d", critical);
        ImGui::PopStyleColor();
        ImGui::Spacing();
        
        // Urgent (Orange)
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.0f, 1.0f));
        ImGui::Text("🟠 Urgent (Priority 2): %d", urgent);
        ImGui::PopStyleColor();
        ImGui::Spacing();
        
        // Standard (Green)
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
        ImGui::Text("🟢 Standard (Priority 3): %d", standard);
        ImGui::PopStyleColor();
        
        ImGui::SetWindowFontScale(1.0f);
        ImGui::EndChild();
        
        ImGui::Spacing();
        
        // Next patient card
        ImGui::BeginChild("NextPatient", ImVec2(0, 220), true);
        
        ImGui::SetWindowFontScale(1.4f);
        ImGui::Text("👤 Next Patient to be Treated");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::Separator();
        ImGui::Spacing();
        
        Patient next = backend.getNextPatient();
        if (next.id != -1) {
            ImGui::SetWindowFontScale(1.2f);
            ImGui::Text("ID: %d", next.id);
            ImGui::Text("Name: %s", next.name.c_str());
            ImGui::Text("Age: %d", next.age);
            
            if (next.priority == 1) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
                ImGui::Text("Priority: 🔴 CRITICAL");
                ImGui::PopStyleColor();
            } else if (next.priority == 2) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.0f, 1.0f));
                ImGui::Text("Priority: 🟠 URGENT");
                ImGui::PopStyleColor();
            } else {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
                ImGui::Text("Priority: 🟢 STANDARD");
                ImGui::PopStyleColor();
            }
            
            ImGui::SetWindowFontScale(1.0f);
            ImGui::Spacing();
            
            if (ImGui::Button("✓ Treat This Patient", ImVec2(200, 50))) {
                backend.treatNextPatient();
                strcpy(statusMessage, "✓ Patient treated successfully!");
                showStatus = true;
                statusTimer = 0.0f;
            }
        } else {
            ImGui::SetWindowFontScale(1.2f);
            ImGui::Text("✓ No patients in queue");
            ImGui::SetWindowFontScale(1.0f);
        }
        
        ImGui::EndChild();
        
        // Quick action buttons
        ImGui::Spacing();
        ImGui::SetWindowFontScale(1.1f);
        if (ImGui::Button("➕ Register New Patient", ImVec2(250, 50))) {
            currentScreen = REGISTER;
        }
        ImGui::SameLine();
        if (ImGui::Button("📋 View Full Queue", ImVec2(250, 50))) {
            currentScreen = QUEUE;
        }
        ImGui::SameLine();
        if (ImGui::Button("📁 All Records", ImVec2(250, 50))) {
            currentScreen = RECORDS;
        }
        ImGui::SetWindowFontScale(1.0f);
        
        ImGui::PopFont();
    }
    
    void renderRegistration() {
        ImGui::SetWindowFontScale(1.5f);
        ImGui::Text("➕ Patient Registration");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::Separator();
        ImGui::Spacing();
        
        ImGui::BeginChild("RegistrationForm", ImVec2(700, 580), true);
        
        ImGui::SetWindowFontScale(1.2f);
        
        // Patient Name
        ImGui::Text("Patient Name:");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PushItemWidth(600);
        ImGui::InputText("##name", nameInput, 128);
        ImGui::PopItemWidth();
        ImGui::Spacing();
        ImGui::Spacing();
        
        // Age
        ImGui::SetWindowFontScale(1.2f);
        ImGui::Text("Age:");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PushItemWidth(600);
        ImGui::InputText("##age", ageInput, 16, ImGuiInputTextFlags_CharsDecimal);
        ImGui::PopItemWidth();
        ImGui::Spacing();
        ImGui::Spacing();
        
        // Symptoms
        ImGui::SetWindowFontScale(1.2f);
        ImGui::Text("Symptoms/Reason for Visit:");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PushItemWidth(600);
        ImGui::InputTextMultiline("##symptoms", symptomsInput, 256, ImVec2(600, 120));
        ImGui::PopItemWidth();
        ImGui::Spacing();
        ImGui::Spacing();
        
        // Priority Level
        ImGui::SetWindowFontScale(1.2f);
        ImGui::Text("Priority Level:");
        ImGui::SetWindowFontScale(1.1f);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
        ImGui::RadioButton("🔴 Critical (Heart attack, Severe trauma)", &selectedPriority, 1);
        ImGui::PopStyleColor();
        
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.0f, 1.0f));
        ImGui::RadioButton("🟠 Urgent (High fever, Fractures)", &selectedPriority, 2);
        ImGui::PopStyleColor();
        
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
        ImGui::RadioButton("🟢 Standard (Minor injuries, Routine)", &selectedPriority, 3);
        ImGui::PopStyleColor();
        ImGui::SetWindowFontScale(1.0f);
        
        ImGui::Spacing();
        ImGui::Spacing();
        
        // Submit Button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
        if (ImGui::Button("✓ Submit Registration", ImVec2(250, 50))) {
            if (validateRegistrationForm()) {
                registerPatient();
            }
        }
        ImGui::PopStyleColor();
        
        ImGui::SameLine();
        
        // Clear Button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.3f, 0.2f, 1.0f));
        if (ImGui::Button("✗ Clear Form", ImVec2(200, 50))) {
            clearRegistrationForm();
        }
        ImGui::PopStyleColor();
        
        // Status message
        if (showStatus) {
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::SetWindowFontScale(1.3f);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
            ImGui::Text("%s", statusMessage);
            ImGui::PopStyleColor();
            ImGui::SetWindowFontScale(1.0f);
        }
        
        ImGui::EndChild();
    }
    
    void renderQueue() {
        ImGui::SetWindowFontScale(1.5f);
        ImGui::Text("📋 Patient Queue (Ordered by Priority)");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::Separator();
        ImGui::Spacing();
        
        auto queue = backend.getQueuedPatients();
        
        if (queue.empty()) {
            ImGui::SetWindowFontScale(1.3f);
            ImGui::Text("✓ No patients currently in queue");
            ImGui::SetWindowFontScale(1.0f);
            return;
        }
        
        ImGui::Text("Total patients in queue: %d", (int)queue.size());
        ImGui::Spacing();
        
        // Table display with larger font
        ImGui::SetWindowFontScale(1.1f);
        if (ImGui::BeginTable("QueueTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY, ImVec2(0, 600))) {
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 80);
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 200);
            ImGui::TableSetupColumn("Age", ImGuiTableColumnFlags_WidthFixed, 80);
            ImGui::TableSetupColumn("Priority", ImGuiTableColumnFlags_WidthFixed, 150);
            ImGui::TableSetupColumn("Symptoms", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();
            
            for (const auto& patient : queue) {
                ImGui::TableNextRow();
                
                ImGui::TableNextColumn();
                ImGui::Text("%d", patient.id);
                
                ImGui::TableNextColumn();
                ImGui::Text("%s", patient.name.c_str());
                
                ImGui::TableNextColumn();
                ImGui::Text("%d", patient.age);
                
                ImGui::TableNextColumn();
                if (patient.priority == 1) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
                    ImGui::Text("🔴 Critical");
                } else if (patient.priority == 2) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.0f, 1.0f));
                    ImGui::Text("🟠 Urgent");
                } else {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
                    ImGui::Text("🟢 Standard");
                }
                ImGui::PopStyleColor();
                
                ImGui::TableNextColumn();
                ImGui::TextWrapped("%s", patient.symptoms.c_str());
            }
            
            ImGui::EndTable();
        }
        ImGui::SetWindowFontScale(1.0f);
    }
    
    void renderSearch() {
        ImGui::SetWindowFontScale(1.5f);
        ImGui::Text("🔍 Search Patient by ID");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::Separator();
        ImGui::Spacing();
        
        ImGui::BeginChild("SearchArea", ImVec2(700, 600), true);
        
        ImGui::SetWindowFontScale(1.2f);
        ImGui::Text("Enter Patient ID:");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PushItemWidth(400);
        ImGui::InputText("##searchID", searchIDInput, 16, ImGuiInputTextFlags_CharsDecimal);
        ImGui::PopItemWidth();
        ImGui::Spacing();
        
        if (ImGui::Button("🔍 Search", ImVec2(200, 50))) {
            performSearch();
        }
        
        ImGui::SameLine();
        
        if (ImGui::Button("✗ Clear", ImVec2(200, 50))) {
            searchIDInput[0] = '\0';
            searchResult = nullptr;
            searchPerformed = false;
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        // Display search results
        if (searchPerformed) {
            if (searchResult != nullptr) {
                ImGui::SetWindowFontScale(1.3f);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
                ImGui::Text("✓ Patient Found");
                ImGui::PopStyleColor();
                ImGui::SetWindowFontScale(1.0f);
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                
                ImGui::SetWindowFontScale(1.2f);
                ImGui::Text("ID: %d", searchResult->id);
                ImGui::Text("Name: %s", searchResult->name.c_str());
                ImGui::Text("Age: %d", searchResult->age);
                
                if (searchResult->priority == 1) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
                    ImGui::Text("Priority: 🔴 CRITICAL");
                    ImGui::PopStyleColor();
                } else if (searchResult->priority == 2) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.0f, 1.0f));
                    ImGui::Text("Priority: 🟠 URGENT");
                    ImGui::PopStyleColor();
                } else {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
                    ImGui::Text("Priority: 🟢 STANDARD");
                    ImGui::PopStyleColor();
                }
                
                ImGui::Text("Symptoms: %s", searchResult->symptoms.c_str());
                ImGui::SetWindowFontScale(1.0f);
            } else {
                ImGui::SetWindowFontScale(1.3f);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
                ImGui::Text("✗ Patient not found!");
                ImGui::PopStyleColor();
                ImGui::SetWindowFontScale(1.0f);
            }
        }
        
        ImGui::EndChild();
    }
    
    void renderAllRecords() {
        ImGui::SetWindowFontScale(1.5f);
        ImGui::Text("📁 All Patient Records (BST)");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::Separator();
        ImGui::Spacing();
        
        auto records = backend.getAllRecords();
        
        if (records.empty()) {
            ImGui::SetWindowFontScale(1.3f);
            ImGui::Text("No patient records found");
            ImGui::SetWindowFontScale(1.0f);
            return;
        }
        
        ImGui::Text("Total records: %d", (int)records.size());
        ImGui::Spacing();
        
        ImGui::SetWindowFontScale(1.1f);
        if (ImGui::BeginTable("RecordsTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY, ImVec2(0, 600))) {
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 80);
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 200);
            ImGui::TableSetupColumn("Age", ImGuiTableColumnFlags_WidthFixed, 80);
            ImGui::TableSetupColumn("Priority", ImGuiTableColumnFlags_WidthFixed, 150);
            ImGui::TableSetupColumn("Symptoms", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();
            
            for (const auto& pd : records) {
                ImGui::TableNextRow();
                
                ImGui::TableNextColumn();
                ImGui::Text("%d", pd.patientID);
                
                ImGui::TableNextColumn();
                ImGui::Text("%s", pd.name.c_str());
                
                ImGui::TableNextColumn();
                ImGui::Text("%d", pd.age);
                
                ImGui::TableNextColumn();
                if (pd.priorityLevel == 1) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
                    ImGui::Text("🔴 Critical");
                } else if (pd.priorityLevel == 2) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.0f, 1.0f));
                    ImGui::Text("🟠 Urgent");
                } else {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
                    ImGui::Text("🟢 Standard");
                }
                ImGui::PopStyleColor();
                
                ImGui::TableNextColumn();
                ImGui::TextWrapped("%s", pd.symptoms.c_str());
            }
            
            ImGui::EndTable();
        }
        ImGui::SetWindowFontScale(1.0f);
    }
    
    bool validateRegistrationForm() {
        if (strlen(nameInput) == 0) {
            strcpy(statusMessage, "✗ Error: Name cannot be empty!");
            showStatus = true;
            statusTimer = 0.0f;
            return false;
        }
        
        if (strlen(ageInput) == 0 || atoi(ageInput) <= 0) {
            strcpy(statusMessage, "✗ Error: Please enter a valid age!");
            showStatus = true;
            statusTimer = 0.0f;
            return false;
        }
        
        if (strlen(symptomsInput) == 0) {
            strcpy(statusMessage, "✗ Error: Symptoms cannot be empty!");
            showStatus = true;
            statusTimer = 0.0f;
            return false;
        }
        
        return true;
    }
    
    void registerPatient() {
        Patient newPatient;
        newPatient.id = backend.getNextID();
        newPatient.name = std::string(nameInput);
        newPatient.age = atoi(ageInput);
        newPatient.symptoms = std::string(symptomsInput);
        newPatient.priority = selectedPriority;
        
        backend.addPatient(newPatient);
        
        sprintf(statusMessage, "✓ Patient registered successfully! ID: %d", newPatient.id);
        showStatus = true;
        statusTimer = 0.0f;
        
        clearRegistrationForm();
    }
    
    void clearRegistrationForm() {
        nameInput[0] = '\0';
        ageInput[0] = '\0';
        symptomsInput[0] = '\0';
        selectedPriority = 2;
    }
    
    void performSearch() {
        if (strlen(searchIDInput) == 0) {
            searchResult = nullptr;
            searchPerformed = false;
            return;
        }
        
        int searchID = atoi(searchIDInput);
        searchResult = backend.searchPatient(searchID);
        searchPerformed = true;
    }
};

int main() {
    if (!glfwInit()) return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(1200, 800, "Hospital Emergency Management System", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    // Enhanced dark theme
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    BackendInterface backend;
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