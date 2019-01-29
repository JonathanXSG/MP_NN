#ifndef _NEURAL_NETWORK_HPP_
#define _NEURAL_NETWORK_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "json.hpp"
#include "Matrix.hpp"
#include "Layer.hpp"

using json = nlohmann::json;

enum NN_COST {
    COST_MSE,
    COST_CEE
};

enum GRADIENT_DESCENT {
    STOCHASTIC = 0,
    MINI_BATCH = 1,
    BATCH = 2
};

struct ANNConfig {
    std::vector<unsigned> topology;
    double bias;
    double learningRate;
    double momentum;
    int epochs;
    int iterations;
    NN_ACTIVATION hActivation;
    NN_ACTIVATION oActivation;
    NN_COST costFunction;
    GRADIENT_DESCENT gradientDescent;
    int batch;
    std::string dataFile;
    std::string labelsFile;
    std::string weightsFile;
    std::string reportFile;
};

class NeuralNetwork {
public:
    explicit NeuralNetwork(ANNConfig config);

    void train(
            std::vector<std::vector<double>>& input,
            std::vector<std::vector<double>>& target
    );

    void test(
            std::vector<std::vector<double>>& input,
            std::vector<std::vector<double>>& target
    );

    void setCurrentInput(std::vector<double>& input);

    void setCurrentTarget(std::vector<double>& target);

    void feedForward();

    void backPropagation();

    void setErrors();
    
    std::vector<double>* getNeurons(unsigned index) {
        return this->layers.at(index)->getNeurons();
    }

    std::vector<double>* getActivatedNeurons(unsigned index) {
        return this->layers.at(index)->getActivatedValues();
    }

    std::vector<double>* getDerivedNeurons(unsigned index) {
        return this->layers.at(index)->getActivatedValues();
    }

    Matrix *getNeuronMatrix(unsigned index) {
        return this->layers.at(index)->matrixifyValues();
    }

    Matrix *getActivatedNeuronMatrix(unsigned index) {
        return this->layers.at(index)->matrixifyActivatedValues();
    }

    Matrix *getDerivedNeuronMatrix(unsigned index) {
        return this->layers.at(index)->matrixifyDerivedValues();
    }

    Matrix *getWeightMatrix(unsigned index) {
        return this->weightMatrices.at(index);
    };
    
    void setLayer(unsigned indexLayer,std::vector<double>& neurons) {
        this->layers.at(indexLayer)->setNeurons(&neurons);
    }

    void saveWeights(std::string file);

    void loadWeights(std::string file);

    unsigned topologySize;
    NN_ACTIVATION hiddenActivationType = A_RELU;
    NN_ACTIVATION outputActivationType = A_SIGM;
    NN_COST costFunctionType = COST_MSE;
    GRADIENT_DESCENT gradientDescent = STOCHASTIC;

    std::vector<unsigned> topology;
    std::vector<Layer *> layers;
    std::vector<Matrix *> weightMatrices;
    std::vector<Matrix *> deltaMatrices;

    std::vector<double>* target;
    std::vector<double> errors;
    std::vector<double> derivedErrors;

    double error = 0;
    double bias = 1;
    double momentum;
    double learningRate;

    ANNConfig config;

private:
    void setErrorMSE();

    void setErrorCEE();
};

#endif
