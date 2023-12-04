#include <iostream>
#include <fstream>
#include "csvstream.hpp"
#include <map>
#include <set>
#include <cmath>

using namespace std;

class Classifier{
    private:
        // {{label, word}, number_of_posts_with_label_containing_word}
        map<pair<string, string>, int> label_word_map;
        map<string, int> vocabulary_map;
        map<string, int> label_map;

        double total_number_of_posts;

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
        int get_num_post_word(const string &word){
            return vocabulary_map.find(word)->second;
        }

        // REQUIRES label
        // EFFECTS return number of post with label input
        int get_num_label(const string &label){
            return label_map.find(label)->second;
        }

        // REQUIRES label
        // EFFECTS return number of post with label input
        int get_num_label_word(const string &label, const string &word){
            return label_word_map.find(make_pair(label, word))->second;
        }

        // REQUIRES label
        // EFFECTS return log_prior_probability
        double log_prior_prob(const string &label){
            double n1 = get_num_label(label);
            return log(n1/total_number_of_posts);
        }

        // REQUIRES label
        // EFFECTS return log_prior_likelihood
        double log_likelihood(const string &label, const string &word){
            if (vocabulary_map.find(word) == vocabulary_map.end()){
                return log(1.0 / total_number_of_posts);
            }else if ((label_word_map.find(make_pair(label, word))
                     == label_word_map.end())){

                double n1 = get_num_post_word(word);
                return log(n1 / total_number_of_posts);
            }else {
                double n1 = get_num_label_word(label, word);
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
                << log_prior_prob(label.first)
                << endl;
            }
        }

        void print_classifier_parameters(){
            cout << "classifier parameters:" << endl;
            for (auto &label_word: label_word_map){
                string label = label_word.first.first;
                string word = label_word.first.second;
                int count = label_word.second;

                cout << "  " << label << ":"
                << word << ", count = " << count
                << ", log-likelihood = " << log_likelihood(label, word)
                << endl;
            }
        }

        pair<string, double> compute_most_probable_tag(string &content){
            map<string, double> probability_of_tag_map;
            set<string> words = unique_words(content);

            for (auto &label : label_map){
                double prob_of_tag = log_prior_prob(label.first);
                for (auto &word : words){
                    prob_of_tag += log_likelihood(label.first, word);
                }
                probability_of_tag_map.insert(
                    make_pair(label.first, prob_of_tag)
                    );
            }

            string first_tag = probability_of_tag_map.begin()->first;
            double first_log_prob = probability_of_tag_map.begin()->second;

            pair<string, double> highest_probability = make_pair(first_tag, first_log_prob);

            for (auto &prob : probability_of_tag_map){
                if (prob.second > highest_probability.second){
                    highest_probability = make_pair(prob.first, prob.second);
                }
            }

            return highest_probability;
        }

        void predict_test_data(csvstream &csv_test_in){
            pair<string, double> highest_prob_tag;

            int number_predicted_correct = 0;
            int number_test_data = 0;
            map<string, string> row;

            cout << "test data:" << endl;

            while(csv_test_in >> row){
                number_test_data++;

                string content = row["content"];
                highest_prob_tag = compute_most_probable_tag(content);

                cout << "  correct = " << row["tag"] <<  ", "
                << "predicted = " << highest_prob_tag.first << 
                ", log-probability score = " << highest_prob_tag.second
                << endl;

                cout << "  content = " << content << endl << endl;

                if (row["tag"] == highest_prob_tag.first){
                    number_predicted_correct++;
                }
            }

            cout << "performance: " << number_predicted_correct << " / "
            << number_test_data << " posts predicted correctly" << endl;
        }

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
    csvstream csv_test_in(test_file);
    classifier.predict_test_data(csv_test_in);

}
