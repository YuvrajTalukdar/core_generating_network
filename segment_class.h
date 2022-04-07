/*
core class handles natural cores
*/
#include<chrono>
#include<random>
#include"neuron_and_ann_class.h"
#include"core_class.h"

using namespace std;
using namespace std::chrono;

class segment_class{
    private:
    /*GENETIC ALGORITHM COMPONENTS START*/
    //gene range
    static const int fp_change_value_min=30,fp_change_value_max=60;
    static const int summation_temp_thershold_min=1500,summation_temp_thershold_max=5000;
    static const int rhs_upper_min=2,rhs_upper_max=20;
    static const int rhs_lower_min=40,rhs_lower_max=150;
    static const int attributes_per_core_min=8,attributes_per_core_max=30;
    float data_div_min=2,data_div_max;
    //algorithm critical data
    unsigned int population_size,ga_iterations,mutation_percentage;
    int no_of_genes_to_mutate;
    vector<chromosome> population;
    //other mics data
    int current_chromosome_id=0;

    //mics functions
    void save_chromosome(chromosome& chromosome);
    void print_population(vector<chromosome>& population);
    int get_random_number(int min,int max);
    bool get_random_bool();
    static bool comparator(chromosome c1,chromosome c2);
    //genetic algorithm critical functions
    void mutation(vector<chromosome>& population);
    void chromosome_data_transfer(int crossover_index,bool before,chromosome& source,chromosome& destination);
    vector<chromosome> crossover(vector<chromosome>& population);
    vector<chromosome> tournament_selection(vector<chromosome> population);
    void generate_initial_population();
    void calc_fitness_threaded(int no_of_threads,vector<chromosome>& population);
    chromosome start_genetic_algorithm(int no_of_threads);
    /*GENETIC ALGORITHM COMPONENTS END*/
    
    
    vector<core_class*> core_vector;
    vector<string> core_save_file_name_vector;

    nn_core_data_package_class* data_pack;
    
    //network information
    float data_division=1.5;
    datapack_structure_defination ds;
    int train_test_predict;
    string network_save_file_name;
    bool id_lock=false;
    int required_no_of_threads;
    int no_of_attributes_per_core_default=25;//25
    int min_no_of_attributes_per_core=4;
    int extra_attributes_in_last_core=0;
    int no_of_attributes_per_core_balanced;//last core may have few extra attributes.

    //segment identification information
    int segment_no=0,segment_aim=0;//this two must be changed using a function so that proper core is loaded
    string segment_name;
    string segment_save_file_name="NULL";//provided if core is loaded from a core/network savefile. Not set using constructor

    //progress bar data
    int predict_progress_bar_numerator=0;//for the predict progress bar
    int predict_progress_bar_denominator=0;//for the predict progress bar

    //genetic algorithm variables
    bool critical_variables_set=false;
    chromosome critical_variable;

    void save_segment();

    bool check_if_datapack_has_valid_labels(nn_core_data_package_class* data_pack);//if a label is 0 than it is invalid

    bool load_segment_if_available(int segment_aim,int segment_no,bool file_name_received,string file_name);

    //void set_lower_firing_constrain_rhs();

    void datapack_analyzer(nn_core_data_package_class* data_pack);//it fills up the datapack_structure_defination ds after analyzing the datapack. 
    struct shuffling_data{
        vector<float> temp_data;
        int temp_label;
    };

    void shuffler(nn_core_filtered_data* f_data);

    void f_data_viewer(string str,vector<nn_core_filtered_data> f_data);

    void filter(nn_core_data_package_class* data_pack);

    bool ds_ns_have_same_labels(core_class* core);

    vector<nn_core_filtered_data> f_data_vector;
    vector<nn_core_filtered_data> f_test_data_vector;
    vector<vector<nn_core_filtered_data>> f_train_data_split;

    void split_attributes_for_each_core();

    void create_cores();

    void calculate_critical_variables(int no_of_threads);

    void train(int no_of_threads,chromosome& current_critical_variable);    

    void start_trainer(nn_core_data_package_class* data_pack,int no_of_threads);//there cannot be a case of invalid network and data without labels.

    void checker_df(vector<neuron> &output_neurons);

    void checker_nf(vector<neuron> &output_neurons);

    int index_of_neuron_to_be_fired(int label,vector<float> elements);

    vector<neuron> combine_output_neurons(vector<vector<neuron>> output_neuron_matrix);

    int propagate(vector<float> input);

    //memory_optimization1
    float testing_for_each_label(/*,int train_test_predict*/);
    
    void print_prediction(nn_core_data_package_class* data_pack,int train_test_predict);

    void predict_progress_bar();

    void make_prediction_on_user_entered_data();

    string message;
    void print_message();
    void clrscr();

    //test functions
    void display_f_train_data_split()
    {
        for(int a=0;a<f_train_data_split.size();a++)
        {
            cout<<"\n\ncore= "<<a;
            for(int b=0;b<f_train_data_split[a].size();b++)
            {
                cout<<"\nlabel= "<<f_train_data_split[a][b].label;
                for(int c=0;c<f_train_data_split[a][b].data.size()-(f_train_data_split[a][b].data.size()-5);c++)
                {
                    cout<<"\n";
                    for(int d=0;d<f_train_data_split[a][b].data[c].size();d++)
                    {
                        cout<<f_train_data_split[a][b].data[c][d]<<",";
                    }
                }
            }
        }
    }

    public:

    void set_critical_variable(chromosome critical_cariable);

    void set_ga_settings(unsigned int &iterations,unsigned int &population_size,unsigned int &mutation_percentage);

    void start_segment(int no_of_threads);//train_test_predict=1//train_test_predic is required for extra assurance

    void add_data(nn_core_data_package_class* data_pack,int train_test_predict,string network_save_file_name);
    
    string return_name()
    {   return segment_name;}
    segment_class(int segment_aim1,int segment_no1,string segment_name1);
};