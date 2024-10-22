#include "dataset.h"
#include "list/listheader.h"
#include "ann/xtensor_lib.h"

#ifndef DATALOADER_H
#define DATALOADER_H

template<typename DType, typename LType>
class DataLoader{
private:
    Dataset<DType, LType>* ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    int m_seed;

    xvector<Batch<DType, LType>> batches;
    xt::xarray<unsigned long> id;
    
    class Iterator {
    private:
        DataLoader<DType, LType>* loader;
        int index;
    public:
        Iterator(DataLoader<DType, LType>* loader, int index) {
            this->loader = loader;
            this->index  = index;
        }

        bool operator!=(const Iterator& other) const { return index != other.index; }

        Iterator& operator++() {
            index++;
            return *this;
        }

        Iterator& operator++(int) {
            Iterator ps = *this; // previous state
            index++;
            return ps;
        }

        Batch<DType, LType> operator*() {
            auto idArray = loader->getIdArray();
            return loader->getBatch(index);
        }
    };

public:
    DataLoader(Dataset<DType, LType>* ptr_dataset,
            int batch_size,
            bool shuffle=true,
            bool drop_last=false,
            int seed = -1) : batches(nullptr, Batch<DType, LType>::batchEQ){
        /*TODO: Add your code to do the initialization */
        this->ptr_dataset = ptr_dataset;
        this->batch_size  = batch_size;
        this->shuffle     = shuffle;
        this->drop_last   = drop_last;
        m_seed            = seed;

        id = xt::arange(0, ptr_dataset->len(), 1);

        if (shuffle) {
            xt::random::seed((m_seed >= 0) ? m_seed : time(nullptr));
            xt::random::shuffle(id);
        }

        // compress into Batch objects
        int J = 0;
        for (int i = 0; i < ptr_dataset->len(); i = J) {
            // copy batch_size (samples) into data
            if (drop_last) {
                J = min(i + batch_size, ptr_dataset->len());
            } else {
                if (ptr_dataset->len() - i < batch_size * 2) {
                    J = ptr_dataset->len();
                } else {
                    J = i + batch_size;
                }
            }

            auto dataShape = ptr_dataset->get_data_shape();
            auto labelShape = ptr_dataset->get_label_shape();

            dataShape[0] = J - i; // number of smaller datasets in the batch

            xt::xarray<DType> data(dataShape);
            xt::xarray<LType> label(labelShape);

            for (int j = i; j < J; j++) {
                DataLabel<DType, LType> jthSample = ptr_dataset->getitem(id(j));

                xt::xarray<DType> jthSampleData = jthSample.getData();
                xt::view(data, j - i, xt::all()) = jthSampleData;

                xt::xarray<LType> jthSampleLabel = jthSample.getLabel();
                // still returns xarray -> get index 0
                if (label.dimension() > 0 && jthSampleLabel.dimension() > 0)
                    label(j - i) = jthSampleLabel(0);
            }

            batches.add(Batch<DType, LType>(data, label));
        }
    }
    virtual ~DataLoader(){}

    Batch<DType, LType> getBatch(int index) { return batches.get(index); }
    xt::xarray<unsigned long> getIdArray() { return id; }

    Iterator begin() { return Iterator(this, 0); }
    Iterator end() { return Iterator(this, batches.size()); }
};

#endif