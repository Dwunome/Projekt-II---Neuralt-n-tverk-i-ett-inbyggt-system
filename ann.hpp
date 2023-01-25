/********************************************************************************
 * Implementering av artificial neural network
 ********************************************************************************/
#ifndef ANN_HPP_
#define ANN_HPP_
#include "dense_layer.hpp"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

/********************************************************************************
 * Klassen ann används för implementering av ett neuralt nätverk. 
 * Innehåller ett ingångslager och ett utgångslager, samt möjliget att välja 
 * antalet dolda lager. 
 ********************************************************************************/
class ann
{
private:
   std::vector<dense_layer> hidden_layers_;     
   dense_layer output_layer_;                   
   std::vector<std::vector<double>> train_in_;  
   std::vector<std::vector<double>> train_out_; 
   std::vector<std::size_t> train_order_;       
   bool train_done_;

   dense_layer &first_hidden_layer(void)
   {
      return hidden_layers_[0];
   }

   dense_layer &last_hidden_layer(void)
   {
      return hidden_layers_[hidden_layers_.size() - 1];
   }

   /********************************************************************************
    * I feedforward beräknas de nya utsignalerna för alla noder i nätverket. 
    *    Input = En referens till vektorn innehållande ny indata.
    ********************************************************************************/
   void feedforward(const std::vector<double> &input)
   {
      first_hidden_layer().feedforward(input);

      for (std::size_t i = 1; i < hidden_layers_.size(); ++i)
      {
         this->hidden_layers_[i].feedforward(hidden_layers_[i - 1].output);
      }
      this->output_layer_.feedforward(this->last_hidden_layer().output);
      return;
   }

   /********************************************************************************
    * I backpropagation beräknas alla noders aktuella fel. 
    *    Reference = En referens till vektorn innehållande korrekta värden.
    ********************************************************************************/
   void backpropagate(const std::vector<double> &reference)
   {
      output_layer_.backpropagate(reference);
      last_hidden_layer().backpropagate(output_layer_);

      for (std::size_t i = hidden_layers_.size() - 1; i > 0; --i)
      {
         hidden_layers_[i - 1].backpropagate(hidden_layers_[i]);
      }

      return;
   }

   /********************************************************************************
    * I optimize sker en justering av alla parametrar i nätverket, för att minska 
    * uppkommet fel vid nästkommande prediktion och ge en högre precision. 
    ********************************************************************************/
   void optimize(const std::vector<double> &input,
                 const double learning_rate)
   {
      first_hidden_layer().optimize(input, learning_rate);

      for (std::size_t i = 1; i < hidden_layers_.size(); ++i)
      {
         this->hidden_layers_[i].optimize(this->hidden_layers_[i - 1].output, learning_rate);
      }

      this->output_layer_.optimize(this->last_hidden_layer().output, learning_rate);
      return;
   }

   /********************************************************************************
    * check_training_data_size används för att kontrollera att antalet
    * träningsuppsättningar med indata är samma som träningsdatan med utdata. 
    * Om antalet ej är samma kortas den större av för att matcha den 
    * mindre uppsättningen.
    ********************************************************************************/
   void check_training_data_size(void)
   {
      if (train_in_.size() != train_out_.size())
      {
         const auto num_sets = train_in_.size() <= train_out_.size() ? train_in_.size() : train_out_.size();
         train_in_.resize(num_sets);
         train_out_.resize(num_sets);
      }
      return;
   }

   /********************************************************************************
    * Initiering av vektor  innehållande ordningsföljden för träningsuppsättningarna
    ********************************************************************************/
   void init_training_order(void)
   {
      this->train_order_.resize(this->train_in_.size());

      for (std::size_t i = 0; i < this->train_order_.size(); ++i)
      {
         this->train_order_[i] = i;
      }
      return;
   }

   /********************************************************************************
    * Ranomiserar ordningsföljden för träningsuppsättningarna. Detta för att 
    * nätverket inte ska vänja sig med träningsdatan, och på så sätt ge
    * bättre precision. 
    ********************************************************************************/
   void randomize_training_order(void)
   {
      for (std::size_t i = 0; i < this->train_order_.size(); ++i)
      {
         const auto r = std::rand() % this->train_order_.size();
         const auto temp = this->train_order_[i];
         this->train_order_[i] = this->train_order_[r];
         this->train_order_[r] = temp;
      }
      return;
   }

public:
   ann(void) {}

   /********************************************************************************
    * Initiering av neurala nätverket med angivet antal noder i respektive lager.
    ********************************************************************************/
   ann(const std::size_t num_inputs,
       const std::size_t num_hidden_layers,
       const std::size_t num_hidden_nodes,
       const std::size_t num_outputs)
   {
      this->init(num_inputs, num_hidden_layers, num_hidden_nodes, num_outputs);
      return;
   }

   /********************************************************************************
    * Destruktor som automatiskt tömmer det neurala nätverket när det går ur scope.
    ********************************************************************************/
   ~ann(void)
   {
      this->clear();
      return;
   }

   /********************************************************************************
    * Returnerar en referens till det dolda lagret så att användaren kan läsa 
    * innehållet, men inte skriva.
    * 
    * Gäller alla nio nedanstående funktioner.
    ********************************************************************************/
   const std::vector<dense_layer> &hidden_layer(void) const
   {
      return this->hidden_layers_;
   }

   const dense_layer &output_layer(void) const
   {
      return this->output_layer_;
   }

   const std::vector<std::vector<double>> &train_in(void) const
   {
      return this->train_in_;
   }

   const std::vector<std::vector<double>> &train_out(void) const
   {
      return this->train_out_;
   }

   std::size_t num_inputs(void)
   {
      return first_hidden_layer().num_weights();
   }

   std::size_t num_hidden_layers(void) const
   {
      return this->hidden_layers_.size();
   }

   std::size_t num_outputs(void) const
   {
      return this->output_layer_.num_nodes();
   }

   std::size_t num_training_sets(void) const
   {
      return this->train_order_.size();
   }

   const std::vector<double> &output(void) const
   {
      return this->output_layer_.output;
   }

   /********************************************************************************
    * Initiering av neurala nätverket med angivet antal noder i respektive lager.
    ********************************************************************************/
   void init(const std::size_t num_inputs,
             const std::size_t num_hidden_layers,
             const std::size_t num_hidden_nodes,
             const std::size_t num_outputs)
   {
      dense_layer first_hidden_layer(num_hidden_nodes, num_inputs);
      hidden_layers_.push_back(first_hidden_layer);

      for (std::size_t i = 1; i < num_hidden_layers; ++i)
      {
         dense_layer hidden_layer(num_hidden_nodes, num_hidden_nodes);
         hidden_layers_.push_back(hidden_layer);
      }

      this->output_layer_.resize(num_outputs, num_hidden_nodes);
      return;
   }

   /********************************************************************************
    * Används av destruktorn för tömmning av nätverket.
    ********************************************************************************/
   void clear(void)
   {
      this->hidden_layers_.clear();
      this->output_layer_.clear();
      this->train_in_.clear();      
      this->train_out_.clear();
      this->train_order_.clear();
      return;
   }

   /********************************************************************************
    * Kopierar innehållet från refererade vektorer för lagring av träningsdata.
    ********************************************************************************/
   void set_training_data(const std::vector<std::vector<double>> &train_in,
                          const std::vector<std::vector<double>> &train_out)
   {
      this->train_in_ = train_in;
      this->train_out_ = train_out;
      this->check_training_data_size();
      this->init_training_order();
      return;
   }

   /********************************************************************************
    * Tränar nätverket under angivet antal epoker och angiven lärhastighet.
    ********************************************************************************/
   void train(const std::size_t num_epochs,
              const double learning_rate)
   {
      for (std::size_t i = 0; i < num_epochs; ++i)
      {
         this->randomize_training_order();

         for (auto &j : this->train_order_)
         {
            const auto &input = this->train_in_[j];
            const auto &reference = this->train_out_[j];

            this->feedforward(input);
            this->backpropagate(reference);
            this->optimize(input, learning_rate);
         }
      }

      train_done_ = true;
      std::cout << "Training done!\n\n";
      return;
   }

   /********************************************************************************
    * Genomför en prediktion av den angivna indatan och returnerar en referens 
    * till vektorn innehållande utdatan.
    ********************************************************************************/
   const std::vector<double> &predict(const std::vector<double> &input)
   {
      this->feedforward(input);
      return this->output();
   }

   /********************************************************************************
    * Genomförd prediktion samt indatan skrivs ut.
    * Denna funktion används dock ej i programmet längre. 
    ********************************************************************************/
   void print(const std::vector<std::vector<double>> &input,
              const std::size_t num_decimals = 1,
              std::ostream &ostream = std::cout)
   {
      if (input.size() == 0)
         return;
      const auto &end = input[input.size() - 1];
      ostream << "--------------------------------------------------------------------------------\n";
      for (auto &i : input)
      {
         ostream << "Input:\t";
         dense_layer::print(i, ostream, num_decimals);

         ostream << "Predicted Output:\t";
         dense_layer::print(this->predict(i), ostream, num_decimals);

         ostream << "LED status:\t";

         if (&i < &end)
            ostream << "\n";
      }
      ostream << "--------------------------------------------------------------------------------\n\n";
      return;
   }

   /********************************************************************************
    * Genomförd prediktion samt indatan skrivs ut.
    * Används ej i programmet. 
    ********************************************************************************/
   void print(const std::size_t num_decimals = 1,
              std::ostream &ostream = std::cout)
   {
      this->print(this->train_in_, num_decimals, ostream);
      return;
   }
};

#endif /* ANN_HPP_ */
