#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <iostream>
#include <fstream>


#pragma comment(lib,"aws-cpp-sdk-core.lib")
#pragma comment(lib,"aws-cpp-sdk-s3.lib")


/**
* Put an object from an Amazon S3 bucket.
*/
int main(int argc, char** argv)
{

	Aws::SDKOptions options;
	options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Debug;
	Aws::InitAPI(options);
	{
		const Aws::String bucket_name = "vertex-team";
		const Aws::String key_name = "myFirstFile.jpg";
		const Aws::String file_name = "captured1.jpg";
		const Aws::String region("us-west-2");

		std::cout << "Uploading " << file_name << " to S3 bucket " <<
			bucket_name << " at key " << key_name << std::endl;

		Aws::Client::ClientConfiguration clientConfig;
		if (!region.empty())
			clientConfig.region = region;

		Aws::S3::S3Client s3_client(clientConfig);

		Aws::S3::Model::PutObjectRequest object_request;
		object_request.WithBucket(bucket_name).WithKey(key_name);

		// Binary files must also have the std::ios_base::bin flag or'ed in
		auto input_data = Aws::MakeShared<Aws::FStream>("PutObjectInputStream",
			file_name.c_str(), std::ios_base::in | std::ios_base::binary);

		object_request.SetBody(input_data);

		auto put_object_outcome = s3_client.PutObject(object_request);

		if (put_object_outcome.IsSuccess())
		{
			std::cout << "Done!" << std::endl;
		}
		else
		{
			std::cout << "PutObject error: " <<
				put_object_outcome.GetError().GetExceptionName() << " " <<
				put_object_outcome.GetError().GetMessage() << std::endl;
		}
	}
	Aws::ShutdownAPI(options);
}