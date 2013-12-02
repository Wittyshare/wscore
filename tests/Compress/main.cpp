/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Tests for the directoryCrawler
 *
 *        Version:  1.0
 *        Created:  07/05/2011 11:26:59 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:
 *        Company:
 *
 * =====================================================================================
 */
#include <assert.h>
#include <Compression/WsCompressor.h>
#include <Compression/WsDecompressor.h>
#include <iostream>
using namespace std;
int main()
{
  WsCompressor comp;
  WsDecompressor decomp;
  char* resp;
  string input = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  long i = comp.compress(input, &resp);
  string output;
  decomp.decompress(resp, i, output);
  assert(input == output);
  input = "hello world";
  i = comp.compress(input, &resp);
  decomp.decompress(resp, i, output);
  assert(input == output);
  input = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer magna nibh, consequat nec sollicitudin id, venenatis ut libero. In ullamcorper varius felis, tristique tristique libero auctor tincidunt. Etiam vehicula risus et nisi tristique ornare. Nullam in sem nec metus bibendum adipiscing. Curabitur ullamcorper faucibus neque, sit amet commodo massa tincidunt quis. Nulla dapibus dui nec urna tincidunt mollis varius odio facilisis. Nulla ut nulla vel purus dapibus iaculis vitae eget quam. Quisque a rhoncus mauris.\
Mauris sagittis sapien et turpis dapibus gravida. Cras consequat orci id elit malesuada non fermentum urna vestibulum. Etiam interdum rutrum urna, vitae accumsan dui mattis at. Mauris turpis felis, tincidunt nec semper vel, congue vitae nulla. Cras ut neque eget ipsum faucibus fringilla nec ut ante. Cras a felis mi, ullamcorper facilisis dolor. Cras id nulla sed ante rutrum pretium. Pellentesque sem enim, tincidunt et egestas at, tempor a ligula.";
  i = comp.compress(input, &resp);
  decomp.decompress(resp, i, output);
  assert(input == output);
  cout << "All compression tests succeeded" << endl;
}
