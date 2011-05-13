.PHONY: src/all src/clean src/depclean src/depend

$(SRC_OBJECTS): $(SRC_DEPS)

src/all: $(SRC_OBJECTS)

src/clean:
	$(RM) $(SRC_OBJECTS)

src/depclean:
	$(RM) $(SRC_DEPS)

src/depend: $(SRC_DEPS)

-include $(SRC_DEPS)
