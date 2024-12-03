function v = OP_FIND()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 66);
  end
  v = vInitialized;
end
