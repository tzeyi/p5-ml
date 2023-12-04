#include <iostream>
#include <fstream>
#include "csvstream.hpp"
#include <map>
#include <set>

using namespace std;

class Classifier{
    private:
        // {{label, word}, number_of_posts_with_label_containing_word}
        map<pair<string, string>, int> label_word_map;
        map<string, int> vocabulary_map;
        map<string, int> label_map;

        int total_number_of_posts;

    public:
        // REQUIRES valid input file name
        // EFFECTS return a set of unique whitespace delimited words
        set<string> unique_words(const string &str) {
            istringstream source(str);
            set<string> words;
            string word;
            while (source >> word) {
                words.insert(word);
            }
            return words;
        }

        // REQUIRES str, label
        // MODIFIES word_label_count
        // EFFECTS insert str and label into their respective maps
        void train_model(const string &label, const string &str){
            total_number_of_posts++;
            
            set<string> words = unique_words(str);

            pair<string, string> key_pair;

            for (const auto&word : words){
                key_pair = make_pair(label, word);
                label_word_map[key_pair]++;

                vocabulary_map[word]++;
            }
            label_map[label]++;
        }

        // EFFECTS return vocabulary size
        int get_vocabulary_size(){
            return vocabulary_map.size();
        }

        // REQUIRES word
        // EFFECTS return number of posts containing w input
        int get_number_of_posts_containting_word(const string &word){
            return vocabulary_map.find(word)->second;
        }

        // REQUIRES label
        // EFFECTS return number of post with label input
        int get_number_of_posts_with_label(const string &label){
            return label_map.find(label)->second;
        }

        // REQUIRES label
        // EFFECTS return number of post with label input
        int get_number_of_posts_with_label_containing_word(const string &label, const string &word){
            return label_word_map.find(make_pair(label, word))->second;
        }

        // REQUIRES label
        // EFFECTS return log_prior_probability
        double compute_log_prior_probability(const string &label){
            double n1 = get_number_of_posts_with_label(label);
            return log(n1/total_number_of_posts);
        }

        // REQUIRES label
        // EFFECTS return log_prior_likelihood
        double compute_log_likelihood(const string &label, const string &word){
            if (vocabulary_map.find(word) == vocabulary_map.end()){
                return log(1 / total_number_of_posts);
            }else if ((label_word_map.find(make_pair(label, word))
                     == label_word_map.end())){

                double n1 = get_number_of_posts_containting_word(word);
                return log(n1 / total_number_of_posts);
            }else {
                double n1 = get_number_of_posts_with_label_containing_word(label, word);
                return log(n1 / label_map.find(label)->second);
            }
        }

        void print_label_content(const string &label, const string &content){
            cout << "  label = " << label <<  ", "
            << "content = " << content << endl;
        }

        void print_training_posts(){
            cout << "trained on " << total_number_of_posts << " examples" << endl;
        }

        void print_vocabulary_size(){
            cout << "vocabulary size = " << get_vocabulary_size() << endl;
        }

        void print_classes(){
            cout << "classes:" << endl;
            for (auto &label: label_map){
                cout << "  " << label.first << ", " << label.second
                << " examples, log-prior = " 
                << compute_log_prior_probability(label.first)
                << endl;
            }
        }

        void print_classifier_parameters(){
            cout << "classifier parameters: " << endl;
            for (auto &label_word: label_word_map){
                string label = label_word.first.first;
                string word = label_word.first.second;
                int count = label_word.second;

                cout << "  " << label << ":"
                << word << ", count = " << count
                << ", log-likelihood = " << compute_log_likelihood(label, word)
                << endl;
            }
        }

        // void print_test_data(const string &label, const string &word){
        //     cout << "test data:" << endl;
        //     cout << "correct = " << label.first << ", " << label.second
        //         << " examples, log-prior = " 
        //         << compute_log_prior_probability(label.first)
        //         << endl;
        //     }
        // }
};


int main(int argc, char* argv[]) {
    cout.precision(3);
    bool debug = false;

    if (argc != 3 && argc != 4){
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1;
    } else if (argc == 4){
        debug = true;

        string debug = argv[3];
        if (debug != "--debug"){
            cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
            return 1;
        }
    }
    
    string train_file = argv[1];
    csvstream csv_train_in(train_file);

    map<string, string> row;
    Classifier classifier;

    if (debug){
        cout << "training data:" << endl;
    }

    while(csv_train_in >> row){
        string label = row["tag"];
        string word = row["content"];
        classifier.train_model(label, word);

        if (debug){
            classifier.print_label_content(label, word); 
        }
    }

    classifier.print_training_posts();

    if (debug){
        classifier.print_vocabulary_size();
        cout << endl;
        classifier.print_classes();
        classifier.print_classifier_parameters();
        cout << endl;
    }


    string test_file = argv[2];
    csvstream csv_test_in(train_file);

}
