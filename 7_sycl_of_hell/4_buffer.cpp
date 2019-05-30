#include <CL/sycl.hpp>

// Inspired by Codeplay compute cpp hello-world
int main(int argc, char** argv) {

   const auto global_range =  (size_t) atoi(argv[1]);
   const auto local_range =  (size_t) atoi(argv[2]);


   // Crrate array
   int A[global_range];

  // Selectors determine which device kernels will be dispatched to.
  cl::sycl::default_selector selector; 
  // Create your own or use `{cpu,gpu,accelerator}_selector`
  {
  // Create sycl buffer.
  // Trivia: What happend if we create the buffer in the outer scope?
  cl::sycl::buffer<cl::sycl::cl_int, 1> bufferA(A, global_range);

  cl::sycl::queue myQueue(selector);
  std::cout << "Running on "
            << myQueue.get_device().get_info<cl::sycl::info::device::name>()
            << "\n";

  //Create a command_group to issue command to the group
  myQueue.submit([&](cl::sycl::handler& cgh) {

     //Create an accesor for the sycl buffer. Trust me, use auto.
     auto accessorA = bufferA.get_access<cl::sycl::access::mode::discard_write>(cgh);

    // Nd range allow use to access information
    cgh.parallel_for<class hello_world>(cl::sycl::nd_range<1>{cl::sycl::range<1>(global_range), 
                                                             cl::sycl::range<1>(local_range) }, 
                                        [=](cl::sycl::nd_item<1> idx) {
      const int world_rank = idx.get_global_id(0);
      accessorA[world_rank] = world_rank;

    }); // End of the kernel function
  }); // End of the queue commands
  }  // End of scope, wait for the queued work to stop. 
     // Can also use  myQueue.wait_and_throw();

 for (size_t i = 0; i < global_range; i++) 
        std::cout<< "A[ " << i <<" ] = " << A[i] << std::endl;
  return 0;
}