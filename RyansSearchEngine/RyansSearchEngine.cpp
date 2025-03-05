#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "index.h"
#include "search.h"

// Hardcoded users for demonstration
std::unordered_map<std::string, std::string> users = {
    {"Ryan", "Curphey"}
};

const std::string USERS_FILE = "users.txt";


void displayStartPrompt() {
    std::cout << "=============================================================" << std::endl;
    std::cout << "  ____                   _                                    " << std::endl;
    std::cout << " |  _ \\ ___  __ _  __ _ (_) __ _  ___   _ __ ___   ___  _ __  " << std::endl;
    std::cout << " | |_) / _ \\/ _` |/ _` || |/ _` |/ _ \\ | '_ ` _ \\ / _ \\| '_ \\ " << std::endl;
    std::cout << " |  _ <  __/ (_| | (_| || | (_| |  __/ | | | | | | (_) | | | |" << std::endl;
    std::cout << " |_| \\_\\___|\\__, |\\__, |_|_|\\__, |\\___| |_| |_| |_|\\___/|_| |_|" << std::endl;
    std::cout << "            |___/    |_|    |___/                            " << std::endl;
    std::cout << "=============================================================" << std::endl;
    std::cout << "               WELCOME TO RYAN'S SEARCH ENGINE                " << std::endl;
    std::cout << "=============================================================" << std::endl;
    std::cout << "  ____                      _                                 " << std::endl;
    std::cout << " / ___|  ___  __ _ _ __ ___| |__   ___  _ __ ___   ___  _ __  " << std::endl;
    std::cout << " \\___ \\ / _ \\/ _` | '__/ __| '_ \\ / _ \\| '_ ` _ \\ / _ \\| '_ \\ " << std::endl;
    std::cout << "  ___) |  __/ (_| | | | (__| | | | (_) | | | | | | (_) | | | |" << std::endl;
    std::cout << " |____/ \\___|\\__,_|_|  \\___|_| |_|\\___/|_| |_| |_|\\___/|_| |_|" << std::endl;
    std::cout << "=============================================================" << std::endl;
}

bool login() {
    std::string username, password;

    std::cout << "Enter username: ";
    std::getline(std::cin, username);

    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    if (users.find(username) != users.end() && users[username] == password) {
        std::cout << "Login successful!" << std::endl;
        return true;
    }
    else {
        std::cout << "Login failed!" << std::endl;
        return false;
    }
}

void saveUsers() {
    std::ofstream file(USERS_FILE);
    if (!file) {
        std::cerr << "Error saving user data!" << std::endl;
        return;
    }

    for (const auto& pair : users) {
        file << pair.first << " " << pair.second << std::endl;
    }
    file.close();
}

void createUser() {
    std::string username, password;

    std::cout << "Enter new username: ";
    std::getline(std::cin, username);

    if (users.find(username) != users.end()) {
        std::cout << "Username already exists. Try again." << std::endl;
        return;
    }

    std::cout << "Enter new password: ";
    std::getline(std::cin, password);

    users[username] = password;
    saveUsers();  // Save to file after adding the user
    std::cout << "User created successfully!" << std::endl;
}


void loadUsers() {
    std::ifstream file(USERS_FILE);
    if (!file) {
        std::cerr << "No user database found. Creating a new one..." << std::endl;
        return;
    }

    std::string username, password;
    while (file >> username >> password) {
        users[username] = password;
    }
    file.close();
}



// Function to calculate the similarity between two strings (simple character match)
int calculateSimilarity(const std::string& str1, const std::string& str2) {
    int matches = 0;
    for (size_t i = 0; i < std::min(str1.size(), str2.size()); ++i) {
        if (str1[i] == str2[i]) {
            ++matches;
        }
    }
    return matches;
}

// Function to suggest questions based on the current query, excluding the current query itself
std::vector<std::pair<int, std::string>> suggestQuestions(const std::vector<std::string>& questions, const std::string& query) {
    std::vector<std::pair<int, std::string>> scoredQuestions;
    for (const auto& question : questions) {
        if (question != query) {
            int score = calculateSimilarity(question, query);
            scoredQuestions.push_back({ score, question });
        }
    }
    std::sort(scoredQuestions.begin(), scoredQuestions.end(), std::greater<>());

    std::vector<std::pair<int, std::string>> suggestions;
    for (size_t i = 0; i < std::min(size_t(3), scoredQuestions.size()); ++i) {
        suggestions.push_back(scoredQuestions[i]);
    }
    return suggestions;
}

// Function to add new questions and answers to the text files
void addNewQuestionAndAnswer(std::vector<std::pair<std::string, std::string>>& qa_pairs, std::vector<std::string>& questions, std::unordered_map<std::string, std::string>& index) {
    std::string newQuestion;
    std::string newAnswer;

    std::cout << "Enter the new question: ";
    std::getline(std::cin, newQuestion);
    std::cout << "Enter the answer: ";
    std::getline(std::cin, newAnswer);

    // Append the new question and answer to the vectors
    qa_pairs.push_back({ newQuestion, newAnswer });
    questions.push_back(newQuestion);
    index[newQuestion] = newAnswer;

    // Open the files in append mode and write the new question and answer
    std::ofstream questionsFile("questions.txt", std::ios::app);
    std::ofstream answersFile("answers.txt", std::ios::app);

    if (questionsFile.is_open() && answersFile.is_open()) {
        questionsFile << newQuestion << std::endl;
        answersFile << newAnswer << std::endl;
    }
    else {
        std::cerr << "Error opening questions or answers file!" << std::endl;
    }

    questionsFile.close();
    answersFile.close();
}

// Function to format and display the answer
void displayAnswer(const std::string& answer) {
    std::string border(answer.size() + 4, '=');

    std::cout << std::endl;
    std::cout << border << std::endl;
    std::cout << "= " << answer << " =" << std::endl;
    std::cout << border << std::endl;
    std::cout << std::endl;
}

// Function to format and display the unknown question message
void displayUnknownMessage() {
    std::string message = "Sorry, I don't know that one.";
    std::string border(message.size() + 4, '-');

    std::cout << std::endl;
    std::cout << border << std::endl;
    std::cout << "| " << message << " |" << std::endl;
    std::cout << border << std::endl;
    std::cout << std::endl;
}

// Function to search for a query in the index, allowing partial matches
std::string searchPartial(const std::unordered_map<std::string, std::string>& index, const std::string& query) {
    for (const auto& pair : index) {
        if (pair.first.find(query) != std::string::npos) {
            return pair.second;
        }
    }
    return "";
}

int main() {
	loadUsers();
    displayStartPrompt();

    char choice;
    std::cout << "=============================================================" << std::endl;
    std::cout << "Do you want to (l)ogin or (c)reate a new user? ";
    std::cin >> choice;
    std::cin.ignore(); // To ignore the newline character left in the buffer

    if (choice == 'c') {
        createUser();
    }

    // Attempt login
    if (!login()) {
        return 1; // Exit if login fails
    }

    // Read questions and answers from files
    std::ifstream questionsFile("questions.txt");
    std::ifstream answersFile("answers.txt");
    std::vector<std::pair<std::string, std::string>> qa_pairs;
    std::vector<std::string> questions;
    std::string question;
    std::string answer;

    if (!questionsFile.is_open() || !answersFile.is_open()) {
        std::cerr << "Error opening questions or answers file!" << std::endl;
        return 1;
    }

    while (std::getline(questionsFile, question) && std::getline(answersFile, answer)) {
        qa_pairs.push_back({ question, answer });
        questions.push_back(question);
    }

    questionsFile.close();
    answersFile.close();

    // Create the index
    auto index = createIndex(qa_pairs);

    // Perform the search with suggestions
    std::string lastQuery;
    while (true) {
        std::string query;
        if (!lastQuery.empty()) {
            std::cout << "Last search: " << lastQuery << std::endl;
        }
        std::cout << "Enter a search query,\n"
            << "type 'add' to add a new question,\n"
            << "or 'exit' to quit: ";
        std::getline(std::cin, query);

        if (query == "exit") {
            break;
        }
        else if (query == "add") {
            addNewQuestionAndAnswer(qa_pairs, questions, index);
            continue;
        }

        std::string result = searchPartial(index, query);

        // Display the search result
        if (!result.empty()) {
            displayAnswer(result);
        }
        else {
            displayUnknownMessage();
        }

        // Loop for suggestions
        while (true) {
            // Suggest similar questions
            auto suggestions = suggestQuestions(questions, query);
            if (!suggestions.empty()) {
                std::cout << "Would you also like to know:" << std::endl;
                for (size_t i = 0; i < suggestions.size(); ++i) {
                    std::cout << "  " << i + 1 << ". " << suggestions[i].second << std::endl;
                }

                // Ask if the user wants to choose one of the suggestions
                std::cout << "Enter the number of the question you'd like to know the answer to, or '0' to skip: ";
                int choice;
                std::cin >> choice;
                std::cin.ignore(); // To ignore the newline character left in the buffer

                if (choice > 0 && choice <= suggestions.size()) {
                    std::string selectedQuestion = suggestions[choice - 1].second;
                    std::string selectedAnswer = searchPartial(index, selectedQuestion);
                    if (!selectedAnswer.empty()) {
                        displayAnswer(selectedAnswer);
                        query = selectedQuestion; // Update the query to the selected question for new suggestions
                    }
                    else {
                        displayUnknownMessage();
                    }
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }

        lastQuery = query;
    }

    return 0;
}
