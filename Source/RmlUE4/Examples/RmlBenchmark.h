#pragma once
#include "RmlDocument.h"
#include "RmlBenchmark.generated.h"

UCLASS()
class URmlBenchmark : public URmlDocument
{
	GENERATED_BODY()
public:

protected:
	// ~Begin URmlDocument API 
	virtual void OnInit() override;
	virtual void OnKeyDown() override;
	virtual void Tick(float DeltaTime) override;
	// ~End URmlDocument API
private:
	void PerformanceTest();
public:
	bool bDoPerformanceTest = false;
private:
	bool run_update = true;
	bool single_update = true;
};
