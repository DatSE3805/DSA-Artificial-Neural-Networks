    #ifndef DATALOADER_H
    #define DATALOADER_H
    #include "ann/xtensor_lib.h"
    #include "ann/dataset.h"
    using namespace std;

    template<typename DType, typename LType>
    class DataLoader {
    public: 
        class Iterator;
        class Batch;

    private:
        Dataset<DType, LType>* ptr_dataset;
        int batch_size;
        bool shuffle;
        bool drop_last;
        xt::xarray<unsigned long> index;
        int m_seed;

    public:
        DataLoader(Dataset<DType, LType>* ptr_dataset, int batch_size, bool shuffle = true, bool drop_last = false, int seed = -1) {
            this->ptr_dataset = ptr_dataset;
            this->batch_size = batch_size;
            this->shuffle = shuffle;
            this->drop_last = drop_last;
            this->m_seed = seed;
            
            index = xt::arange<unsigned long>(0, ptr_dataset->len());

            if (shuffle) {
                if (m_seed >= 0) {
                    xt::random::seed(m_seed);
                    xt::random::shuffle(index);
                }
            }
        }

        class Batch {
        private:
            xt::xarray<DType> data;
            xt::xarray<LType> label;
            bool HasLabel;

        public:
            Batch(const xt::xarray<DType>& data, const xt::xarray<LType>& label) : data(data), label(label), HasLabel(true) {}
            const xt::xarray<DType>& getData() const { return data; }
            const xt::xarray<LType>& getLabel() const { return label; }
            Batch(const xt::xarray<DType>& data) : data(data), label(), HasLabel(false) {}
            bool hasLabel() const { return HasLabel; }
        };

        Batch getBatch(int IndexBatch) {
            int start = IndexBatch * batch_size;
            int end = start + batch_size;
            bool PresentLabel = false;

            if (!drop_last) {
                if (IndexBatch == ((ptr_dataset->len() / batch_size) - 1))
                    end = index.size();
            }

            auto data_shape = ptr_dataset->get_data_shape();
            auto label_shape = ptr_dataset->get_label_shape();
            data_shape[0] = end - start;
            
            xt::xarray<DType> batch_data = xt::zeros<DType>(data_shape);
            xt::xarray<LType> batch_label;
            
            if (!label_shape.empty()) {
                PresentLabel = true;
                label_shape[0] = end - start;
                batch_label = xt::zeros<LType>(label_shape);
            }

            for (int i = 0; i < end - start; i++) {
                auto item = ptr_dataset->getitem(index[start + i]);
                xt::view(batch_data, i) = item.getData();
                if (PresentLabel) {
                    if (label_shape.size() == 1)
                        batch_label(i) = item.getLabel()(0);
                    else
                        xt::view(batch_label, i) = item.getLabel();
                }
            }

            if (PresentLabel)
                return Batch(batch_data, batch_label);
            else
                return Batch(batch_data);
        }

        virtual ~DataLoader(){ }
        /////////////////////////////////////////////////////////////////////////
        // The section for supporting the iteration and for-each to DataLoader //
        /// START: Section                                                     //
        /////////////////////////////////////////////////////////////////////////

        class Iterator {
        private:
            DataLoader* loader;
            int IndexBatch;
        
        public:
            Iterator(DataLoader* loader, int IndexBatch) : loader(loader), IndexBatch(IndexBatch) {}

            bool operator != (const Iterator& another) const {
                return IndexBatch != another.IndexBatch;
            }

            bool operator==(const Iterator& other) const {
                return IndexBatch == other.IndexBatch;
            }

            Iterator& operator++() {
                IndexBatch++;
                return *this;
            }

            Iterator operator++(int) {
                Iterator temp = *this;
                ++(*this);
                return temp;
            }

            Batch operator*() {
                return loader->getBatch(IndexBatch);
            }
        };

        Iterator begin() {
            return Iterator(this, 0);
        }

        Iterator end() {
            int totalbatches = ptr_dataset->len() / batch_size;
            return Iterator(this, totalbatches);
        }
        /////////////////////////////////////////////////////////////////////////
        // The section for supporting the iteration and for-each to DataLoader //
        /// END: Section                                                       //
        /////////////////////////////////////////////////////////////////////////
    };

    #endif /* DATALOADER_H */