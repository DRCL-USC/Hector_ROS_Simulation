function v = OP_SUBASSIGN()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 85);
  end
  v = vInitialized;
end
