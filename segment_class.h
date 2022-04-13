/*
core class handles natural cores
*/
#include<chrono>
#include<random>
#include"neuron_and_ann_class.h"
#include"core_class.h"

using namespace std;
using namespace std::chrono;

class genetic_algorithm
{
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
    public:
    datapack_structure_defination ds;
    vector<nn_core_filtered_data>* f_data_vector;

    chromosome start_genetic_algorithm(int no_of_threads);
    genetic_algorithm(unsigned int &iterations,unsigned int &population_size,unsigned int &mutation_percentage,int &data_div_max);
    /*GENETIC ALGORITHM COMPONENTS END*/
};

class segment_class{
    private:
    vector<core_class*> core_vector;
    vector<string> core_save_file_name_vector;
    
    //network information
    float data_division=1.5;
    datapack_structure_defination ds;
    string network_save_file_name;
    bool id_lock=false;
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

    void save_segment();

    void f_data_viewer(string str,vector<nn_core_filtered_data> f_data);

    void filter(nn_core_data_package_class* data_pack);

    bool ds_ns_have_same_labels(core_class* core);

    vector<nn_core_filtered_data> f_data_vector;
    vector<nn_core_filtered_data> f_test_data_vector;
    vector<vector<nn_core_filtered_data>> f_train_data_split;

    void split_attributes_for_each_core();

    void create_cores();

    void checker_df(vector<neuron> &output_neurons);

    void checker_nf(vector<neuron> &output_neurons);

    int index_of_neuron_to_be_fired(int label,vector<float> elements);

    vector<neuron> combine_output_neurons(vector<vector<neuron>> output_neuron_matrix);

    int propagate(vector<float> input);

    void predict_progress_bar();

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
    int no_of_threads;
    chromosome* critical_variable;

    void start_trainer();//there cannot be a case of invalid network and data without labels.
    
    void train();

    float testing_for_each_label(/*,int train_test_predict*/);

    bool load_segment_if_available(string file_name);

    void print_prediction(nn_core_data_package_class& data_pack,int train_test_predict);

    void make_prediction_on_user_entered_data();

    void set_ds(datapack_structure_defination ds1)
    {   ds=ds1;}

    void set_critical_variable(chromosome* critical_cariable);

    void add_f_data(vector<nn_core_filtered_data>& f_data_vec);

    void clear();
    
    string return_name()
    {   return segment_name;}
    datapack_structure_defination return_ds()
    {   return ds;}
    segment_class(int segment_aim1,int segment_no1,string segment_name1);
};