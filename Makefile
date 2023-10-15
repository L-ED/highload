# Rest service ---- ---- ---- ---- ----

.PHONY: rest-image
rest-image:
	docker build --rm --no-cache -t rest-service:hl -f docker/rest-service/Dockerfile .

.PHONY: build-rest
build-rest:
	cmake --build build --target rest-service -- -j4

.PHONY: run-rest
run-rest:
	./run-rest.sh

# Other service ---- ---- ---- ---- ----

.PHONY: build-other
build-other:
	cmake --build build --target other-service -- -j4

.PHONY: run-other
run-other:
	./run-other.sh

# Database ---- ---- ---- ---- ----

.PHONY: run-db
run-db:
	docker run --name database --detach --rm -p 3306:3306 \
		--env MYSQL_DATABASE=hl \
		--env MYSQL_USER=postgres \
		--env MYSQL_PASSWORD=postgres \
		--env MYSQL_ROOT_PASSWORD=postgres \
		mariadb:hl 
