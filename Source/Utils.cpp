/*
  ==============================================================================

    Utils.cpp
    Created: 27 Apr 2019 12:30:25am
    Author:  Emilio Molina

  ==============================================================================
*/

#include "Utils.h"

bool isReadableAudioFile(const File& inputFile,
                         AudioFormatManager& formatManager) {
    auto* reader = formatManager.createReaderFor (inputFile);
    bool isValid = (reader != nullptr);
    delete reader;
    return isValid;
}


void readFromAudioFile(const File& inputAudioFile,
                       AudioSampleBuffer& buffer,
                       AudioFormatManager& formatManager) {
    auto* reader = formatManager.createReaderFor(inputAudioFile);
    if (reader != nullptr)
    {
        //auto duration = reader->lengthInSamples / reader->sampleRate;
        buffer.setSize (reader->numChannels, (int) reader->lengthInSamples);
        reader->read (&buffer,
                      0,  // destination start sample
                      (int) reader->lengthInSamples,  // num samples
                      0,  // reading start sample
                      true,  // true: read left channel
                      true); // true: read right channel
        delete reader;
    }
}

void writeToAudioFile(AudioSampleBuffer& buffer,
                      int sampleRate,
                      String outputAudioPath) {
    if (File(outputAudioPath).exists()) {
        remove(outputAudioPath.toRawUTF8());
    }
    WavAudioFormat format;
    std::unique_ptr<AudioFormatWriter> writer;
    writer.reset (format.createWriterFor (new FileOutputStream (outputAudioPath),
                                          sampleRate,
                                          buffer.getNumChannels(),
                                          24,
                                          {},
                                          0));
    if (writer != nullptr)
        writer->writeFromAudioSampleBuffer (buffer, 0, buffer.getNumSamples());
}


void vectorToBuffer(std::vector<float> &inputL,
                    std::vector<float> &inputR,
                    AudioSampleBuffer &output) {
    output.clear();
    int numChannels = 2;
    int numSamples = (int)inputL.size();
    output.setSize (numChannels, numSamples);
    if (numSamples > 0) {
        output.copyFrom(0, 0, &inputL[0], (int)inputL.size());
        output.copyFrom(1, 0, &inputR[0], (int)inputR.size());
    }
}



void medianFilter( float* array, int n, int filterSize )
{
    Mediator<float> mediator( filterSize );
    for (int i = 0; i < filterSize/2; i++)
    {
        mediator.insert( array[0] );
        array[i] = mediator.getMedian();
    }
    int offset = filterSize/2 + (filterSize % 2);
    for (int i = 0; i < offset; i++)
    {
        mediator.insert( array[i] );
    }
    for (int i = 0; i < n - offset; i++)
    {
        array[i] = mediator.getMedian();
        mediator.insert( array[i + offset] );
    }
    for (int i = n - offset; i < n; i++)
    {
        array[i] = mediator.getMedian();
        mediator.insert( array[n - 1] );
    }
}


template<typename T>
std::vector<T> slice(std::vector<T> const &v, int m, int n)
{
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + n + 1;
    
    std::vector<T> vec(first, last);
    return vec;
}


void medianfilterSymmetric(std::vector<float>& input,
                           std::vector<float>& output,
                           int wsize) {
    int N = (int)input.size();
    output.clear();
    output.reserve(N + wsize * 2);
    for (int i=wsize; i>=0; i--) {
        output.push_back(input[i]);
    }
    for (int i=0; i<N; i++) {
        output.push_back(input[i]);
    }
    for (int i=N - 1; i>=(N - wsize + 1); i--) {
        output.push_back(input[i]);
    }
    medianFilter(&output[0], (int)output.size(), wsize);
    output = slice(output,
                   wsize + 1,
                   (int)output.size() - wsize);
}


void horizontalMedianFiltering(std::vector<std::vector<float>> &input,
                               std::vector<std::vector<float>> &output,
                               int N) {
    output.clear();
    for (auto &row: input) {
        std::vector<float> outputRow(row.size(), 0.0f);
        medianfilterSymmetric(row,
                              outputRow,
                              N);
        output.push_back(outputRow);
    }
}

void transpose(std::vector<std::vector<float> > &b)
{
    std::vector<std::vector<float> > trans_vec(b[0].size(), std::vector<float>(b.size()));
    for (int i = 0; i < b.size(); i++)
        for (int j = 0; j < b[i].size(); j++)
            trans_vec[j][i] = b[i][j];
    b = trans_vec;
}

void verticalMedianFiltering(std::vector<std::vector<float>> input,
                             std::vector<std::vector<float>> &output,
                             int N) {
    transpose(input);
    output.clear();
    for (auto &row: input) {
        std::vector<float> outputRow(row.size(), 0.0f);
        medianfilterSymmetric(row,
                              outputRow,
                              N);
        output.push_back(outputRow);
    }
    transpose(output);
}

void soft_mask(std::vector<std::vector<float>> &X,
               std::vector<std::vector<float>> &X_ref,
               float margin_factor,
               std::vector<std::vector<float>> &mask,
               float power=1.0f,
               bool split_zeros=false) {
    mask.resize(X.size());
    for (auto &row: mask)
        row.resize(X[0].size());
    float x, xref, z, m, rm;
    for (int i=0; i<X.size(); i++) {
        for (int j=0; j<X[i].size(); j++) {
            x = X[i][j];
            xref = X_ref[i][j] * margin_factor;
            z = std::max(x, xref);
            if (z < 1e-8) {
                if (split_zeros)
                    m = 0.5f;
                else
                    m = 0.0f;
            } else {
                m = std::pow((x / z), power);
                rm = std::pow((xref / z), power);
                m = m / (m + rm);
            }
            mask[i][j] = m;
        }
    }
}


void hpss(std::vector<std::vector<float>> &X,
          int win_harm,
          int win_perc,
          float power,
          float margin_harm,
          float margin_perc,
          std::vector<std::vector<float>> &harm,
          std::vector<std::vector<float>> &perc) {
    horizontalMedianFiltering(X, harm, win_harm);
    verticalMedianFiltering(X, perc, win_perc);
    bool split_zeros = (margin_harm == 1.0f) && (margin_perc == 1.0f);
    std::vector<std::vector<float>> mask_harm(X);
    std::vector<std::vector<float>> mask_perc(X);
    soft_mask(harm, perc, margin_harm, mask_harm, power, split_zeros);
    soft_mask(perc, harm, margin_perc, mask_perc, power, split_zeros);
    for (int i=0; i<X.size(); i++) {
        for (int j=0; j<X[i].size(); j++) {
            harm[i][j] *= mask_harm[i][j];
            perc[i][j] *= mask_perc[i][j];
        }
    }
}
