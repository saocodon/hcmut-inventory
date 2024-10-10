#ifndef DATASET_H
#define DATASET_H

#include "xtensor_lib.h"

template<typename DType, typename LType>
class DataLabel{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
public:
    DataLabel(xt::xarray<DType> data,  xt::xarray<LType> label):
    data(data), label(label){
    }
    xt::xarray<DType> getData() const{ return data; }
    xt::xarray<LType> getLabel() const{ return label; }
};

template<typename DType, typename LType>
class Batch{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
public:
    Batch(xt::xarray<DType> data,  xt::xarray<LType> label):
    data(data), label(label){
    }
    Batch() : data(xt::xarray<DType>()), label(xt::xarray<LType>()) {}

    virtual ~Batch(){}
    xt::xarray<DType>& getData(){return data; }
    xt::xarray<LType>& getLabel(){return label; }


    bool operator==(Batch<DType, LType> rhs) {
        return ((rhs.data == data) && (rhs.label == label));
    }

    static bool batchEQ(Batch<DType, LType>& lhs, Batch<DType, LType>& rhs) { return lhs == rhs; }
    static bool batchEQ(Batch<DType, LType>*& lhs, Batch<DType, LType>*& rhs) { return *lhs == *rhs; }

    // Overload the << operator for Batch
    friend std::ostream& operator<<(std::ostream& os, const Batch<DType, LType>& batch) {
        os << "Batch Data:\n";
        os << batch.data; // Assuming xt::xarray has an overloaded operator<<
        os << "Batch Labels:\n";
        os << batch.label; // Assuming xt::xarray has an overloaded operator<<
        return os;
    }
};

template<typename DType, typename LType>
class Dataset{
private:
public:
    Dataset(){};
    virtual ~Dataset(){};

    virtual int len()=0;
    virtual DataLabel<DType, LType> getitem(int index)=0;
    virtual xt::svector<unsigned long> get_data_shape()=0;
    virtual xt::svector<unsigned long> get_label_shape()=0;

};

template<typename DType, typename LType>
class TensorDataset: public Dataset<DType, LType> {
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
    xt::svector<unsigned long> data_shape, label_shape;
public:
    TensorDataset(xt::xarray<DType> data, xt::xarray<LType> label) {
        this->data = data;
        this->label = label;
        data_shape = data.shape();
        label_shape = label.shape();
    }
    int len() { return data_shape[0]; }
    DataLabel<DType, LType> getitem(int index) {
        if (index < 0 || index >= data_shape[0])
            throw std::out_of_range("Index is out of range!");

        if (data.dimension() != 0 && label.dimension() != 0 && data_shape[0] == label_shape[0])
            return DataLabel<DType, LType>(data(index), label(index));
        else
            return DataLabel<DType, LType>(data(index), xt::xarray<LType>());
    }
    xt::svector<unsigned long> get_data_shape() { return data_shape; }
    xt::svector<unsigned long> get_label_shape() { return label_shape; }
};

#endif