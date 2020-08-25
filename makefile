all:
	@echo "compile library"
	make -C src/comlib/
	@echo "compile pub"
	make -C src/pub/
	@echo "compile sub"
	make -C src/sub/
	ldconfig

clean:
	make clean -C src/comlib/  
	make clean -C src/pub/  
	make clean -C src/sub/  
	
