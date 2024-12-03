function v = OP_TWICE()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 29);
  end
  v = vInitialized;
end
