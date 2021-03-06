package socialnetwork.repository.file;

import socialnetwork.domain.Entity;
import socialnetwork.domain.validators.Validator;
import socialnetwork.repository.memory.InMemoryRepository;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.sql.Array;
import java.util.Arrays;
import java.util.List;
import java.util.Optional;

public abstract class AbstractFileRepository<ID, E extends Entity<ID>> extends InMemoryRepository<ID, E>{
    String fileName;

    public AbstractFileRepository(String fileName, Validator<E> validator){
        super(validator);
        this.fileName=fileName;
        loadData();
    }

    private void loadData(){
        Path path = Paths.get(fileName);
        try{
            List<String> lines = Files.readAllLines(path);
            lines.forEach(
                    linie-> {
                        E entity = extractEntity(Arrays.asList(linie.split(";")));
                        super.save(entity);
                    }
            );
        }
        catch (IOException e){
            e.printStackTrace();
        }
    }

    /**
     *  extract entity  - template method design pattern
     *  creates an entity of type E having a specified list of @code attributes
     * @param attributes
     * @return an entity of type E
     */
    public abstract E extractEntity(List<String> attributes);

    protected abstract String createEntityAsString(E entity);

    @Override
    public Optional<E> save(E entity) {
        Optional<E> obj = super.save(entity);
        if(obj.isPresent())
            return obj;
        writeToFile(entity);
        return Optional.empty();
    }

    @Override
    public Optional<E> delete(ID id) {
        Optional<E> obj = super.delete(id);
        if(!obj.isPresent())
            return Optional.empty();

        try{
            BufferedWriter buffer = new BufferedWriter(new FileWriter(fileName, false));
            for(E entity : this.findAll()) {
                buffer.write(createEntityAsString(entity));
                buffer.newLine();;
            }
            buffer.close();
        }
        catch(IOException e){
            e.printStackTrace();
        }

        return obj;
    }

    @Override
    public Optional<E> update(E entity) {
        Optional<E> obj = super.update(entity);
        if(!obj.isPresent())
            return Optional.empty();

        try{
            BufferedWriter buffer = new BufferedWriter(new FileWriter(fileName, false));
            for(E ent : this.findAll()) {
                buffer.write(createEntityAsString(ent));
                buffer.newLine();;
            }
            buffer.close();
        }
        catch(IOException e){
            e.printStackTrace();
        }

        return obj;
    }

    protected void writeToFile(E entity){
        try{
            BufferedWriter buffer = new BufferedWriter(new FileWriter(fileName, true));
            buffer.write(createEntityAsString(entity));
            buffer.newLine();;
            buffer.close();
        }
        catch(IOException e){
            e.printStackTrace();
        }
    }

    @Override
    public int size() {
        return super.size();
    }
}