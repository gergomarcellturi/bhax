// Semantic MNIST (Experiment 1a, no-centering) The generator program
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
// g++ smnistg-no-centering.cpp -o smnistg -lpng
// ./smnistg train-labels-idx1-ubyte train-images-idx3-ubyte 60000
// ./smnistg t10k-labels-idx1-ubyte t10k-images-idx3-ubyte 10000
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

#include <iostream>
#include "png++/png.hpp"
#include <random>
#include <fstream>
#include <arpa/inet.h>

const int max_digit = 9;
const uint32_t size = 28;
std::random_device rd;
//std::default_random_engine re ( rd() );
std::default_random_engine re; // debug
std::uniform_int_distribution < int > sem_value_uniform_dist ( 0, max_digit );
std::uniform_int_distribution < int > coords_uniform_dist ( 0 + 4, size - 4 );

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

png::image < png::rgb_pixel > create_img ( int size, int sem_value )
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

    for ( int j {0}; j < sem_value; ++j ) {

        int ok = 1;

        do {

            x[j] = coords_uniform_dist ( re );
            y[j] = coords_uniform_dist ( re );

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

// no-centering                
//   int meanx = sumx/sem_value;
//   int meany = sumy/sem_value;

//   int xshift = size/2 - meanx;
//   int yshift = size/2 - meany;

    for ( int j {0}; j < sem_value; ++j ) {

        for ( int k {-1}; k < 2; ++k )
            for ( int l {-1}; l < 2; ++l ) {
                
                img.set_pixel ( x[j]+k, y[j]+l, png::rgb_pixel ( 0, 0, 0 ) );                
                
// no-centering                
//                if ( x[j]+xshift+k>=0 && x[j]+xshift+k<size
//                        && y[j]+yshift+l>=0 && y[j]+yshift+l<size ) {
//                    img.set_pixel ( x[j]+xshift+k, y[j]+yshift+l, png::rgb_pixel ( 0, 0, 0 ) );
//                } else {
//                    img.set_pixel ( x[j]+k, y[j]+l, png::rgb_pixel ( 0, 0, 0 ) );
//                }


            }

    }

    return img;
}

int
main ( int argc, char *argv[] )
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

    const int img_name_size = 48;
    char img_name[img_name_size];

    for ( uint32_t i {0}; i < number_of_images; ++i ) {

        int sem_value = sem_value_uniform_dist ( re );
        label = sem_value;
        labels.write ( ( char * ) &label, sizeof ( uint8_t ) );

        png::image < png::rgb_pixel > img = create_img ( size, sem_value );

        write_image ( images, img );

        std::sprintf ( img_name, "smnist-%d-%d.png", sem_value, i );
        img.write ( img_name );
        std::cout << "\r" << img_name << " saved" << std::flush;

    }

    labels.close();
    images.close();
}
