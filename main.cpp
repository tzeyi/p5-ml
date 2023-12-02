#include <iostream>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <fstream>
#include "csvstream.hpp"

using namespace std;

class Classifier{
   private: 
      map<pair<string, string>, int> my_map; //<<label, word>, count>
      set<string> setof_unique_tags;
      set<string> setof_all_unique_words;
      int total_posts;
      int vocabulary_count;
      string words_database_string;
      map<string, int> tag_freq;


   public:

    //REQUIRE: str to not be empty
    //MODIFY: N/A
    //EFFECT: Returns a set of strings of unique (non-duplicate) words.
    set<string> unique_words(const string &str) {
        istringstream source(str);
        set<string> words;
        string word;
        while (source >> word) {
            words.insert(word);
        }
        return words;
    }

    //REQUIRE: label and content to not be NULL
    //MODIFY: my_map
    //EFFECT: Initializes my_map with the data from the training sets; Increase the number
    //        of posts; construct a word database string.
    void init_mymap (const string &label, const string &content){
        setof_unique_tags.insert(label);

        set<string> setof_unique_words;
        setof_unique_words = unique_words(content);

        for (auto i: setof_unique_words) {
            my_map[{label, i}]++;
        }

        total_posts++;

        words_database_string = words_database_string + " " + content;
    }
    
    //REQUIRE: label to not be NULL
    //MODIFY: tag_freq
    //EFFECT: Increases tag frequency by one
    void increase_tag_freq (const string &label){
        tag_freq[label]++;
    }

    //REQUIRE: word to not be NULL
    //MODIFY: N/A
    //EFFECT: Returns the number of posts containing word w. 
    int total_posts_containing_word (const string &word){
        int count = 0;
        for (auto i : setof_unique_tags){
            auto j = my_map.find(make_pair(i, word));
            if (j != my_map.end()) {
                count += j->second;
            }
        }
        return count;
    }

    //REQUIRE: label to not be NULL
    //MODIFY: N/A
    //EFFECT: returns ln(P(C))
    double lnPC (string label){

        double total_posts_d = (double)total_posts;
        double total_post_labelC = tag_freq[label];

        return log(total_post_labelC/total_posts_d);
    }

    //REQUIRE: label and string to not be NULL
    //MODIFY: N/A
    //EFFECT: returns ln(P(W|C))
    double lnPWC (const string &label, const string &word){
        int pair_count;
        auto i = my_map.find(make_pair(label, word));
        if (i == my_map.end()){
            pair_count = -1;
        } else {
            pair_count = i->second;
        }

        double pair_count_d = (double)pair_count;
        double total_posts_d = (double)total_posts;
        double number_posts_w = total_posts_containing_word (word);   //probably need to double it?
        double total_post_labelC = tag_freq[label];

        if (number_posts_w == 0){
            return log (1.0 / total_posts_d);
        } else if (pair_count == -1){
            return log (number_posts_w / total_posts_d);
        } else {
            return log (pair_count_d / total_post_labelC);
        }
    }

    //REQUIRE: content to not be NULL
    //MODIFY: N/A
    //EFFECT: returns the highest scoring tag as the predicted tag and its log probability 
    pair<string, double> predicted_tag_and_logprob (const string &content){
        vector <string> tags;
        vector <double> log_score; 
        
        set<string> setof_words = unique_words (content);

        for (auto i : setof_unique_tags) {
            tags.push_back(i);
            double tags_score = 0.0;
            tags_score += lnPC(i);
            for (auto j : setof_words) {
                tags_score += lnPWC(i, j); 
            }
            log_score.push_back(tags_score);
        }

        int index_highestscoringtag = 0;
        double highestscore = log_score[0];

        for (size_t i = 1; i < tags.size(); i++) {
            if (log_score[i] > highestscore) {
                highestscore = log_score[i];
                index_highestscoringtag = i;
            } else if (log_score[i] == highestscore) {
                if (tags[index_highestscoringtag].compare(tags[i]) > 0) {
                    highestscore = log_score[i];
                    index_highestscoringtag = i;
                }
            }
        }
        return make_pair(tags[index_highestscoringtag], highestscore);
    }
    
    //REQUIRE: N/A
    //MODIFY: N/A
    //EFFECT: prints the additional details in debug mode. 
    void debugmode_print (){
        setof_all_unique_words = unique_words(words_database_string);
        vocabulary_count = setof_all_unique_words.size();

        cout << "vocabulary size = " << vocabulary_count << "\n\n" << "classes:" << "\n";

        for (auto i : setof_unique_tags) {
            cout << "  ";
            cout << i << ", " << tag_freq[i] << " examples, log-prior = " 
                                                         << lnPC(i) << '\n';
        }
        
        cout << "classifier parameters:" << '\n';

        for (auto i : my_map) {
            cout << "  " << i.first.first << ":" << i.first.second;
            cout << ", count = " << i.second << ", log-likelihood = " 
                         << lnPWC(i.first.first, i.first.second) << '\n';
        }
        cout << '\n';
    }

    int get_total_posts (){
        return total_posts;
    }
};

int main (int argc, char *argv[]){
    
    cout.precision(3);
    Classifier classi = Classifier ();

    string trainfile = string(argv[1]);
    csvstream csvin(trainfile);
    string testfile = string(argv[2]);
    csvstream csvout(testfile);

    if (argc != 3 && argc != 4) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1;
    }
    
    bool debugmode = false;

    if (argc == 4) {
        if (string(argv[3]) == "--debug") { 
            debugmode = true; 
            cout << "training data:" << '\n';
        } else {
            cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        }
    }
    
    map<string, string> row;
    while (csvin >> row) {
       string tag; string content;
       for (auto &col : row) {
            if (col.first == "tag") {
                tag = col.second;
            } else if (col.first == "content") {
                content = col.second;
            }
        }
        classi.init_mymap(tag, content);
        classi.increase_tag_freq (tag);
        if (debugmode){
            cout << "  label = " << tag << ", content = " << content << '\n';
        }
    }
    
    cout << "trained on " << classi.get_total_posts() << " examples" << '\n';
    if (debugmode) { 
        classi.debugmode_print();
    } else {
        cout << "\n";
    }
    cout << "test data:" << '\n';

    int total_posts_tested = 0;
    int total_posts_accurate = 0;
    while(csvout >> row){
        string tag; string content;
        for (auto &col : row) {
            if(col.first == "tag"){
                tag = col.second;
            } else if(col.first == "content") {
                content = col.second;
            }
        }

        pair <string, double> prediction = classi.predicted_tag_and_logprob(content);
        cout << "  correct = " << tag << ", predicted = " << prediction.first   
                           << ", log-probability score = " << prediction.second
                           << "\n  content = " << content << "\n\n";
        
        total_posts_tested++;
        if (tag == prediction.first){
            total_posts_accurate++;
        }
    }

    cout << "performance: " << total_posts_accurate << " / " << total_posts_tested <<
                                                        " posts predicted correctly\n";  

}