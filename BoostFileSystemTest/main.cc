/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
#include "boost/filesystem.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

namespace bfs = boost::filesystem;

void main()
{
	bfs::path p = bfs::path("/aa/bb/main.jpg");

	std::cout << "extension : " << p.extension().string() << std::endl;
	std::cout << "root_directory : " << p.root_directory().string() << std::endl;
	std::cout << "relative_path : " << p.relative_path().string() << std::endl;
	std::cout << "parent_path : " << p.parent_path().string() << std::endl;
	std::cout << "stem : " << p.stem().string() << std::endl;
	std::cout << "filename : " << p.filename().string() << std::endl;
	std::cout << "root_path : " << p.root_path().string() << std::endl;
	std::cout << "root_name : " << p.root_name().string() << std::endl;

	std::cout << std::endl << "===========================" << std::endl << std::endl;

	bfs::path q = bfs::path("game.tga");

	std::cout << "extension : " << q.extension().string() << std::endl;
	std::cout << "root_directory : " << q.root_directory().string() << std::endl;
	std::cout << "relative_path : " << q.relative_path().string() << std::endl;
	std::cout << "parent_path : " << q.parent_path().string() << std::endl;
	std::cout << "stem : " << q.stem().string() << std::endl;
	std::cout << "filename : " << q.filename().string() << std::endl;
	std::cout << "root_path : " << q.root_path().string() << std::endl;
	std::cout << "root_name : " << q.root_name().string() << std::endl;

	std::cout << std::endl << "===========================" << std::endl << std::endl;

	bfs::path pq = p.parent_path();
	//pq.append(q.stem().string());
	pq /= q.stem();
	pq.concat(".mp3");
	std::cout << "pq.string = " << pq.string() << std::endl;
	std::cout << "pq.generic_string = " << pq.generic_string() << std::endl;

	bfs::path pq2 = p.parent_path();
	pq2 += q.stem();
	std::cout << "pq2.string = " << pq2.string() << std::endl;

	std::cout << std::endl << "===========================" << std::endl << std::endl;

	system("PAUSE");
}