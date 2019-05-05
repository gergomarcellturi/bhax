// Semantic MNIST (Experiment 1b, no-centering, "hard" 1px disjunct test set)
// The generator program
// This program generates images that contains less than 10 points.
//
// Copyright (C) 2019 Norbert Bátfai, batfai.norbert@inf.unideb.hu
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// Version history
//
// Initial hack, the output are binary compatible with the format of
// original MNIST (http://yann.lecun.com/exdb/mnist/) training and test data.
//
// See also https://bhaxor.blog.hu/2019/03/10/the_semantic_mnist and
// http://smartcity.inf.unideb.hu/~norbi/SMNIST
//
// g++ smnistg-hard-disjunct-test-set-no-centering.cpp -o smnistg -lpng
// ./smnistg
//
// bug: 1544 of 60000 train images contain points that have the same coords
//
//  int found = 0;
//
//    for ( int j {0}; j < sem_value; ++j ) {
//
//        x[j] = coords_uniform_dist ( re );
//        y[j] = coords_uniform_dist ( re );
//
//        for ( int k {0}; k < j; ++k ) {
//
//            if ( x[k] == x[j] && y[k] == y[j] && !found ) {
//
//                found = 1;
//                std::cout << " the same point" << std::endl;
//                break;
//
//            }
//        }
//
//        sumx += x[j];
//        sumy += y[j];
//    }
//
// fixed.
//
// At Viktor Simkó's suggestion, the random images that have already been in the training set 
// has been filtered out from the test set. This code satisfied a stronger property that what 
// Viktor gave, namely all generated random images are unique images. It follows that training 
// images are excluded from the test images.
//
// g++ smnistg-hard-1px-disjunct-test-set-no-centering.cpp -o smnistg -lpng
// ./smnistg
//
// "hard": the set of coordinate pairs of pixels is divided into two disjunct sets by the 
// function void init_drnd(double p_train). Then the training images are generated from the 
// one set and the test images are generated from the other set.
// 
// "hard 1px": In the former datasets, dots are 3x3 pixels, now they are 1 pixel
//
// bugfix:   init_drnd ( 1.0 - 1.0/7.0, size );
//

#include <iostream>
#include "png++/png.hpp"
#include <random>
#include <fstream>
#include <arpa/inet.h>
#include <map>
#include <array>
#include <vector>

const int max_digit = 9;
const uint32_t size = 28;
std::random_device rd;
//std::default_random_engine re ( rd() );
std::default_random_engine re; // debug
std::uniform_int_distribution < int > sem_value_uniform_dist ( 0, max_digit );
// hard
//std::uniform_int_distribution < int > coords_uniform_dist ( 0 + 4, size - 4 );
std::uniform_real_distribution < float > p_dist ( 0.0, 1.0 );

bool little_endian()
{
    uint32_t u = 1;
    char *p = ( char * ) &u;
    return  *p == 1;
}

void write_int ( std::fstream & f, uint32_t n )
{
    if ( little_endian() ) {
        n = htonl ( n );
    }
    f.write ( ( char * ) &n, sizeof ( uint32_t ) );
}

void write_image ( std::fstream & f, png::image < png::rgb_pixel > & img )
{
    uint8_t pixel;
    int size = img.get_width();

    for ( int j {0}; j < size; ++j )
        for ( int k {0}; k < size; ++k )
            if ( img.get_pixel ( j, k ).red > 0 ) {
                pixel = 0x00;
                f.write ( ( char * ) &pixel, sizeof ( uint8_t ) );
            } else {
                pixel = 0xff;
                f.write ( ( char * ) &pixel, sizeof ( uint8_t ) );
            }

}

std::vector<int> drnd_test;
std::vector<int> drnd_train;

void init_drnd(double p_train)
{

// hard
//std::uniform_int_distribution < int > coords_uniform_dist ( 0 + 4, size - 4 );

    for ( int j {0+4}; j <= size-4; ++j )
        for ( int k {0+4}; k <= size-4; ++k )
            if(p_dist(re) < p_train)
            {
                drnd_train.push_back(j);
                drnd_train.push_back(k);
            }
            else
            {
                drnd_test.push_back(j);
                drnd_test.push_back(k);
            }

}

void coords_drnd ( int *x, int *y, std::vector<int> & rnd_coords  )
{
    std::uniform_int_distribution < int > coords_hdisjunct_dist ( 0, rnd_coords.size() /2 -1 );

    int idx = coords_hdisjunct_dist ( re );
    *x = rnd_coords[2*idx];
    *y = rnd_coords[2*idx + 1];
}

std::map<int, std::vector<std::array<int, max_digit*2> >> images;

bool was_it_already ( int sem_value, int* x, int* y )
{
    bool wasItAlready {false};
    std::array<int, max_digit*2> arr;

    for ( int i {0}; i<sem_value; ++i ) {
        arr[i*2] = x[i];
        arr[i*2+1] = y[i];
    }

    for ( auto img : images[sem_value] ) {
        if ( arr == img ) {
            wasItAlready = true;
            break;
        }
    }

    if ( !wasItAlready ) {
        images[sem_value].push_back ( arr );
    }

    return wasItAlready;
}

png::image < png::rgb_pixel > create_img ( int size, int sem_value, std::vector<int> & rnd_coords )
{
    png::image < png::rgb_pixel > img ( size, size );

    for ( int j {0}; j < size; ++j )
        for ( int k {0}; k < size; ++k ) {
            img.set_pixel ( j, k, png::rgb_pixel ( 255, 255, 255 ) );
        }

    if ( !sem_value ) {
        return img;
    }

    int x[sem_value];
    int y[sem_value];

// no-centering
//    int sumx = 0;
//    int sumy = 0;

    int itWasAlreadyCnt {0};
    do {

        for ( int j {0}; j < sem_value; ++j ) {

            int ok = 1;

            do {

// hard
//            x[j] = coords_uniform_dist ( re );
//            y[j] = coords_uniform_dist ( re );

                coords_drnd ( x+j, y+j, rnd_coords );

                ok = 1;
                for ( int k {0}; k < j; ++k ) {

                    if ( x[k] == x[j] && y[k] == y[j] ) {

                        ok = 0;
                    }

                }

            } while ( !ok );

// no-centering
//       sumx += x[j];
//       sumy += y[j];
        }


        if(++itWasAlreadyCnt > 10)
            throw std::domain_error( std::string("The domain of images of "+ std::to_string(sem_value) + " dot(s) is probably full."));

    } while ( was_it_already ( sem_value, x, y ) );


// no-centering
//   int meanx = sumx/sem_value;
//   int meany = sumy/sem_value;

//   int xshift = size/2 - meanx;
//   int yshift = size/2 - meany;

    for ( int j {0}; j < sem_value; ++j ) {

// 1px        
//        for ( int k {-1}; k < 2; ++k )
//            for ( int l {-1}; l < 2; ++l ) {
//
//                img.set_pixel ( x[j]+k, y[j]+l, png::rgb_pixel ( 0, 0, 0 ) );

        img.set_pixel ( x[j], y[j], png::rgb_pixel ( 0, 0, 0 ) );        
        
// no-centering
//                if ( x[j]+xshift+k>=0 && x[j]+xshift+k<size
//                        && y[j]+yshift+l>=0 && y[j]+yshift+l<size ) {
//                    img.set_pixel ( x[j]+xshift+k, y[j]+yshift+l, png::rgb_pixel ( 0, 0, 0 ) );
//                } else {
//                    img.set_pixel ( x[j]+k, y[j]+l, png::rgb_pixel ( 0, 0, 0 ) );
//                }


//            }

    }

    return img;
}

int
smnist ( char const *argv[], std::vector<int> & rnd_coords )
{

    uint32_t mn_l = 0x00000801;
    uint32_t mn_i = 0x00000803;
    uint32_t number_of_images = atoi ( argv[3] );
    uint8_t label;

    std::fstream labels ( argv[1], std::ios_base::out );
    std::fstream images ( argv[2], std::ios_base::out );

    write_int ( labels, mn_l );
    write_int ( labels, number_of_images );

    write_int ( images, mn_i );
    write_int ( images, number_of_images );
    write_int ( images, size );
    write_int ( images, size );

    const int img_name_size = 512;
    char img_name[img_name_size];

    //for ( uint32_t i {0}; i < number_of_images; ++i ) {
    uint32_t i {0};
    while ( i < number_of_images ) {

        int sem_value = sem_value_uniform_dist ( re );

        try {

            png::image < png::rgb_pixel > img = create_img ( size, sem_value, rnd_coords );

            write_image ( images, img );

            std::sprintf ( img_name, "%s-%d-%d.png", argv[0], sem_value, i );
            img.write ( img_name );

            label = sem_value;
            labels.write ( ( char * ) &label, sizeof ( uint8_t ) );

            std::cout << "\r" << img_name << " saved" << std::flush;

             ++i;
             
        } catch( const std::domain_error& e ) {

            std::cout << e.what() << std::endl;
            // TODO: remove this sem_value from sem_value_uniform_dist
        }

    }

    labels.close();
    images.close();
}

int
main ( )
{

    init_drnd(1.0 - 1.0/7.0);

    char const *train[]= {
        "smnistg-train", "train-labels-idx1-ubyte", "train-images-idx3-ubyte", "60000"
    };

    smnist ( train, drnd_train );

    char const *test[]= {
        "smnistg-test", "t10k-labels-idx1-ubyte", "t10k-images-idx3-ubyte", "10000"
    };

    smnist ( test, drnd_test );

}
